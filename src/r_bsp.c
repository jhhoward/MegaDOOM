#include "doomtypes.h"
#include "r_local.h"
#include "tables.h"
#include "m_bbox.h"
#include "r_defs.h"

#include "Map.h"


void DrawDebugLine(int x0, int y0, int x1, int y1);
void DrawMapDebugLine(int x0, int y0, int x1, int y1, uint32_t colour);

#ifdef WIN32
#define DEBUG_MAP_ZOOM 5
#define DEBUG_MAP_LINE(x1, y1, x2, y2, colour) \
    DrawMapDebugLine(320 + (x1) / DEBUG_MAP_ZOOM, 200 - (y1) / DEBUG_MAP_ZOOM, 320 + (x2) / DEBUG_MAP_ZOOM, 200 - (y2) / DEBUG_MAP_ZOOM, colour);
#else
#define DEBUG_MAP_LINE(x1, y1, x2, y2, colour)
#endif

//
// ClipWallSegment
// Clips the given range of columns
// and includes it in the new clip list.
//
typedef	struct
{
    int	first;
    int last;

} cliprange_t;


//
// R_CheckBBox
// Checks BSP node/subtree bounding box.
// Returns true
//  if some part of the bbox might be visible.
//
int	checkcoord[12][4] =
{
    {3,0,2,1},
    {3,0,2,0},
    {3,1,2,0},
    {0},
    {2,0,2,1},
    {0,0,0,0},
    {3,1,3,0},
    {0},
    {2,0,3,1},
    {2,1,3,1},
    {2,1,3,0}
};

#define FRONT_CLIP_PLANE 4

void VLine(int x, int y, int count, uint8_t colour);

int16_t floorClip[VIEWPORT_WIDTH];
int16_t ceilingClip[VIEWPORT_WIDTH];
int16_t columnsToFill;

angle_t clipangle;
angle_t negviewangle;
int16_t viewcos;
int16_t viewsin;

#define USE_DIVIDE_INSTRUCTIONS 1

inline int16_t CalculateInterpolant(int16_t x, int16_t range)
{
    return x * reciprocal[range];
}

inline int16_t ClipValue(int16_t x1, int16_t x2, int16_t y1, int16_t y2, int16_t clipx)
{
#if USE_DIVIDE_INSTRUCTIONS
    return y1 + (clipx - x1) * (y2 - y1) / (x2 - x1);
#else
    int16_t range = x2 - x1;
    if (range >= 1024)
        return y1;
    int16_t interpolant = clipx * reciprocal[range];
    return y1 + (((y2 - y1) * interpolant) >> 10);
#endif
}

inline int16_t ClipFOVRight(int16_t x1, int16_t y1, int16_t x2, int16_t y2)
{
#if USE_DIVIDE_INSTRUCTIONS
    return x1 + (x2 - x1) * (x1 - y1) / ((y2 - y1) - (x2 - x1));
#else
    int16_t range = (y2 - y1) - (x2 - x1);
    if (range >= 1024)
        range = 1023;
    int16_t num = (x2 - x1) * (x1 - y1);
    int16_t den = reciprocal[range];
    num >>= 8;
    den >>= 3;
    return x1 + (num * den);
#endif
}

//vx1 += (FRONT_CLIP_PLANE - vy1) * (vx2 - vx1) / (vy2 - vy1);
//vy2 = vx2 = vx1 + (vx2 - vx1) * (vx1 - vy1) / ((vy2 - vy1) - (vx2 - vx1));

//vx1 = vx1 + (vx2 - vx1) * (-vx1 - vy1) / ((vy2 - vy1) + (vx2 - vx1));


inline int16_t ScaleByDistanceX(int16_t x, int16_t z)
{
#if USE_DIVIDE_INSTRUCTIONS
    return (x * VIEWPORT_HALF_WIDTH) / z;
#else
    while (z > 4096)
    {
        z >>= 1;
        x >>= 1;
    }
    return (x * distancescale[z]) >> 10;
#endif
}

inline int16_t ScaleByDistanceY(int16_t x, int16_t z)
{
#if USE_DIVIDE_INSTRUCTIONS
    return (x * VIEWPORT_WIDTH) / z;
#else
    while (z > 4096)
    {
        z >>= 1;
        x >>= 1;
    }
    return (x * distancescale[z]) >> 9;
#endif
}

void R_Subsector(uint16_t subSectorNum)
{
    const subsector_t* ssector = &currentlevel->subsectors[subSectorNum];
    int i;
    int16_t segnum;

    //printf("Render subsector %d\n", subSectorNum);

    segnum = ssector->firstline;

    for (i = 0; i < ssector->numlines; i++, segnum++)
    {
        const seg_t* seg = &currentlevel->segs[segnum];

        if (clipangle < ANG270)
        {
            if (seg->angle >= clipangle && seg->angle < clipangle + ANG90)
            {
                continue;
            }
        }
        else
        {
            if (seg->angle >= clipangle || seg->angle < clipangle - ANG270)
            {
                continue;
            }

        }

        const line_t* linedef = seg->linedef;

        int16_t relvx1 = (seg->v1->x - viewx);
        int16_t relvy1 = (seg->v1->y - viewy);
        int16_t vx1 = ((relvx1 * viewcos) >> 8) - ((relvy1 * viewsin) >> 8);
        int16_t vy1 = ((relvx1 * viewsin) >> 8) + ((relvy1 * viewcos) >> 8);

        //if (vx1 > vy1)
        //{
        //    // Clipped to right side of the view space
        //    continue;
        //}

        int16_t relvx2 = (seg->v2->x - viewx);
        int16_t relvy2 = (seg->v2->y - viewy);
        int16_t vx2 = ((relvx2 * viewcos) >> 8) - ((relvy2 * viewsin) >> 8);
        int16_t vy2 = ((relvx2 * viewsin) >> 8) + ((relvy2 * viewcos) >> 8);

        int16_t sx1, sx2;
        const int maxsx = VIEWPORT_WIDTH - 1;


        DEBUG_MAP_LINE(vx1, vy1, vx2, vy2, 0xff444444);

        //if (vx2 < -vy2)
        //{
        //    // Clipped to the left side of the view space
        //    continue;
        //}

        if (vy1 < FRONT_CLIP_PLANE)
        {
            if (vy2 < FRONT_CLIP_PLANE)
            {
                // Segment entirely behind the clip plane
                continue;
            }

            //vx1 += (FRONT_CLIP_PLANE - vy1) * (vx2 - vx1) / (vy2 - vy1);
            vx1 = ClipValue(vy1, vy2, vx1, vx2, FRONT_CLIP_PLANE);
            vy1 = FRONT_CLIP_PLANE;
        }
        else if (vy2 < FRONT_CLIP_PLANE)
        {
            if (vy1 < FRONT_CLIP_PLANE)
            {
                // Segment entirely behind the clip plane
                continue;
            }
//            vx2 += (FRONT_CLIP_PLANE - vy2) * (vx1 - vx2) / (vy1 - vy2);
            vx2 = ClipValue(vy2, vy1, vx2, vx1, FRONT_CLIP_PLANE);
            vy2 = FRONT_CLIP_PLANE;
        }

        if (vx2 > vy2)
        {
            if (vx1 > vy1)
            {
                // Completely clipped
                continue;
            }
            // Right side needs clipping
            // x' = x0 + (x1 - x0) * ((x0 - y0) / ((y1 - y0) - (x1 - x0)))
            //vy2 = vx2 = vx1 + (vx2 - vx1) * (vx1 - vy1) / ((vy2 - vy1) - (vx2 - vx1));
            vy2 = vx2 = ClipFOVRight(vx1, vy1, vx2, vy2);
            sx2 = maxsx;
        }
        else
        {
            //sx2 = (VIEWPORT_WIDTH / 2) + ((VIEWPORT_WIDTH / 2) * vx2) / vy2;
            sx2 = VIEWPORT_HALF_WIDTH + ScaleByDistanceX(vx2, vy2);
        }
        
        if (vx1 < -vy1)
        {
            if (vx2 < -vy2)
            {
                // Completely clipped
                continue;
            }
            // Left side needs clipping
            // x' = x0 + (x1 - x0) * ((x0 - y0) / ((y1 - y0) - (x1 - x0)))
            vx1 = vx1 + (vx2 - vx1) * (-vx1 - vy1) / ((vy2 - vy1) + (vx2 - vx1));
            //vx1 = vx1 + (((vx2 - vx1) * (-vx1 - vy1) * reciprocal[((vy2 - vy1) + (vx2 - vx1))]) >> 10);
            vy1 = -vx1;
            sx1 = 0;
        }
        else
        {
            //sx1 = (VIEWPORT_WIDTH / 2) + ((VIEWPORT_WIDTH / 2) * vx1) / vy1;
            sx1 = VIEWPORT_HALF_WIDTH + ScaleByDistanceX(vx1, vy1);
        }

        if (sx1 < 0)
            sx1 = 0;
        if (sx2 > maxsx)
            sx2 = maxsx;

        if (sx1 >= sx2)
        {
            // Back face culled
            continue;
        }

        DEBUG_MAP_LINE(vx1, vy1, vx2, vy2, 0xffffffff);

        int shade = 128 + (seg->angle >> 9);
        uint8_t colour;
        uint8_t floorColour = 0x11;
        uint8_t ceilingColour = 0x88;

        //srand(segnum);
        //colour = (uint8_t)rand();
        colour = (uint8_t)(segnum);

        if (linedef->sidenum[0] != -1)
        {
            //mapsidedef_t* side =  &Map.sides[linedef->sidenum[seg->side]];
            //mapsector_t* sector = &Map.sectors[side->sector];
            const side_t* side = seg->sidedef;
            const sector_t* sector = side->sector;

            floorColour = (uint8_t)(sector);
            ceilingColour = (uint8_t)(sector + 1);
            //srand(side->sector + 1024);
            //floorColour = (uint8_t)rand();
            //ceilingColour = (uint8_t)rand();

            //int u1 = (VIEWPORT_HEIGHT / 2) - 128 * (sector->ceilingheight - viewz) / vy1;
            //int u2 = (VIEWPORT_HEIGHT / 2) - 128 * (sector->ceilingheight - viewz) / vy2;
            //int l1 = (VIEWPORT_HEIGHT / 2) - 128 * (sector->floorheight - viewz) / vy1;
            //int l2 = (VIEWPORT_HEIGHT / 2) - 128 * (sector->floorheight - viewz) / vy2;
            int u1 = VIEWPORT_HALF_HEIGHT - ScaleByDistanceY(sector->ceilingheight - viewz, vy1);
            int u2 = VIEWPORT_HALF_HEIGHT - ScaleByDistanceY(sector->ceilingheight - viewz, vy2);
            int l1 = VIEWPORT_HALF_HEIGHT - ScaleByDistanceY(sector->floorheight - viewz, vy1);
            int l2 = VIEWPORT_HALF_HEIGHT - ScaleByDistanceY(sector->floorheight - viewz, vy2);

            int16_t dsx = sx2 - sx1;
            int16_t uerror = dsx >> 1;
            int16_t lerror = dsx >> 1;
            int16_t u = u1;
            int16_t du;
            int16_t ustep;
            int16_t l = l1;
            int16_t dl;
            int16_t lstep;

            if (u1 < u2)
            {
                du = u2 - u1;
                ustep = 1;
            }
            else
            {
                du = u1 - u2;
                ustep = -1;
            }

            if (l1 < l2)
            {
                dl = l2 - l1;
                lstep = 1;
            }
            else
            {
                dl = l1 - l2;
                lstep = -1;
            }

            for (int x = sx1; x <= sx2; x++)
            {
                int y1 = u;
                int y2 = l;
                int16_t clipTop = ceilingClip[x];
                int16_t clipBottom = floorClip[x];

                if (clipTop < clipBottom)// && y2 >= clipTop && y1 <= clipBottom)
                {
                    if (y1 <= clipTop)
                    {
                        y1 = clipTop + 1;
                    }
                    else
                    {
                        if (y1 < clipBottom)
                        {
                            VLine(x, clipTop + 1, y1 - clipTop - 1, ceilingColour);
                        }
                        else
                        {
                            VLine(x, clipTop + 1, clipBottom - clipTop - 1, ceilingColour);
                            ceilingClip[x] = floorClip[x] = 0;
                            columnsToFill--;
                            goto fullyclipped;
                        }
                    }

                    if (y2 > clipBottom)
                    {
                        y2 = clipBottom;
                    }
                    else
                    {
                        if (y2 > clipTop)
                        {
                            VLine(x, y2, clipBottom - y2, floorColour);
                        }
                        else
                        {
                            VLine(x, clipTop + 1, clipBottom - clipTop - 1, floorColour);
                            ceilingClip[x] = floorClip[x] = 0;
                            columnsToFill--;
                            goto fullyclipped;
                        }
                    }

                    //if (u > ceilingClip[x])
                    //{
                    //    VLine(x, ceilingClip[x] + 1, u - ceilingClip[x] + 1, ceilingColour);
                    //}

                    if (side->midtexture)
                    {
                        ceilingClip[x] = floorClip[x] = 0;
                        if (y2 <= y1)
                            continue;
                        VLine(x, y1, y2 - y1, colour);
                        columnsToFill--;
                    }
                    else
                    {
                        ceilingClip[x] = y1 - 1;
                        floorClip[x] = y2;
                    }
                }

                fullyclipped:

                uerror -= du;
                lerror -= dl;

                while (uerror < 0)
                {
                    u += ustep;
                    uerror += dsx;
                }
                while (lerror < 0)
                {
                    l += lstep;
                    lerror += dsx;
                }
            }

            //if (seg->side == 0 && linedef->sidenum[1] != -1)
            if(seg->linedef->backsector)
            {
                // Connected to another sector
                //mapsidedef_t* backside = &Map.sides[linedef->sidenum[1]];
                //mapsector_t* backsector = &Map.sectors[backside->sector];
                
                const sector_t* backsector = seg->linedef->backsector;

                if (backsector->ceilingheight < sector->ceilingheight || backsector->floorheight > sector->floorheight)
                {
                    //u1 = (VIEWPORT_HEIGHT / 2) - 128 * (backsector->ceilingheight - viewz) / vy1;
                    //u2 = (VIEWPORT_HEIGHT / 2) - 128 * (backsector->ceilingheight - viewz) / vy2;
                    //l1 = (VIEWPORT_HEIGHT / 2) - 128 * (backsector->floorheight - viewz) / vy1;
                    //l2 = (VIEWPORT_HEIGHT / 2) - 128 * (backsector->floorheight - viewz) / vy2;
                    u1 = VIEWPORT_HALF_HEIGHT - ScaleByDistanceY(backsector->ceilingheight - viewz, vy1);
                    u2 = VIEWPORT_HALF_HEIGHT - ScaleByDistanceY(backsector->ceilingheight - viewz, vy2);
                    l1 = VIEWPORT_HALF_HEIGHT - ScaleByDistanceY(backsector->floorheight - viewz, vy1);
                    l2 = VIEWPORT_HALF_HEIGHT - ScaleByDistanceY(backsector->floorheight - viewz, vy2);

                    uerror = dsx >> 1;
                    lerror = dsx >> 1;
                    u = u1;
                    l = l1;

                    if (u1 < u2)
                    {
                        du = u2 - u1;
                        ustep = 1;
                    }
                    else
                    {
                        du = u1 - u2;
                        ustep = -1;
                    }

                    if (l1 < l2)
                    {
                        dl = l2 - l1;
                        lstep = 1;
                    }
                    else
                    {
                        dl = l1 - l2;
                        lstep = -1;
                    }

                    for (int x = sx1; x <= sx2; x++)
                    {
                        int y1 = u;
                        int y2 = l;
                        int16_t clipTop = ceilingClip[x];
                        int16_t clipBottom = floorClip[x];

                        if (clipTop < clipBottom) // && y2 >= clipTop && y1 <= clipBottom)
                        {
                            //if (y1 > clipBottom)
                            //    y1 = clipBottom;
                            //if (y2 <= clipTop)
                            //    y2 = clipTop + 1;

                            if (y1 > clipTop)
                            {
                                if (y1 >= clipBottom)
                                {
                                    y1 = clipBottom;
                                }

                                VLine(x, clipTop + 1, y1 - clipTop - 1, colour);
                                ceilingClip[x] = y1 - 1;
                            }

                            if (y2 < clipBottom)
                            {
                                if (y2 <= clipTop)
                                {
                                    y2 = clipTop + 1;
                                }

                                VLine(x, y2, clipBottom - y2, colour);
                                floorClip[x] = y2;
                            }

                        }

                        uerror -= du;
                        lerror -= dl;

                        while (uerror < 0)
                        {
                            u += ustep;
                            uerror += dsx;
                        }
                        while (lerror < 0)
                        {
                            l += lstep;
                            lerror += dsx;
                        }
                    }

                }
            }

        }
    }

}


bool R_CheckBBox(int16_t* bspcoord)
{
    int			boxx;
    int			boxy;
    int			boxpos;

    int16_t		x1;
    int16_t		y1;
    int16_t		x2;
    int16_t		y2;

    angle_t		angle1;
    angle_t		angle2;
    angle_t		span;
    angle_t		tspan;

    cliprange_t* start;

    int			sx1;
    int			sx2;

    // Find the corners of the box
    // that define the edges from current viewpoint.
    if (viewx <= bspcoord[BOXLEFT])
        boxx = 0;
    else if (viewx < bspcoord[BOXRIGHT])
        boxx = 1;
    else
        boxx = 2;

    if (viewy >= bspcoord[BOXTOP])
        boxy = 0;
    else if (viewy > bspcoord[BOXBOTTOM])
        boxy = 1;
    else
        boxy = 2;

    boxpos = (boxy << 2) + boxx;
    if (boxpos == 5)    // Camera is in the center of the bounding box
        return true;

    x1 = bspcoord[checkcoord[boxpos][0]] - viewx;
    y1 = bspcoord[checkcoord[boxpos][1]] - viewy;
    x2 = bspcoord[checkcoord[boxpos][2]] - viewx;
    y2 = bspcoord[checkcoord[boxpos][3]] - viewy;

    int16_t vy1 = ((x1 * viewsin) >> 8) + ((y1 * viewcos) >> 8);
    int16_t vy2 = ((x2 * viewsin) >> 8) + ((y2 * viewcos) >> 8);

    // Check if box is behind the camera
    if (vy1 < FRONT_CLIP_PLANE && vy2 < FRONT_CLIP_PLANE)
        return false;

    int16_t vx1 = ((x1 * viewcos) >> 8) - ((y1 * viewsin) >> 8);
    int16_t vx2 = ((x2 * viewcos) >> 8) - ((y2 * viewsin) >> 8);

    //const int zoom = 5;
    ////DrawMapDebugLine(vx1, vy1, x2, vy2, 0xffcccc00);
    //DrawMapDebugLine(
    //                    320 + (vx1) / zoom,
    //                    200 - (vy1) / zoom,
    //                    320 + (vx2) / zoom,
    //                    200 - (vy2) / zoom, 0xff4455ff);

    // Check if entirely to the left of the camera's FOV
    if (vx1 < -vy1 && vx2 < -vy2)
        return false;
    
    // Check if entirely to the right of the camera's FOV
    if (vx1 > vy1 && vx2 > vy2)
        return false;

    return true;

    /*

    // check clip list for an open space
    angle1 = R_PointToAngle(x1, y1) - viewangle;
    angle2 = R_PointToAngle(x2, y2) - viewangle;

    span = angle1 - angle2;

    // Sitting on a line?
    if (span >= ANG180)
        return true;

    tspan = angle1 + clipangle;

    if (tspan > 2 * clipangle)
    {
        tspan -= 2 * clipangle;

        // Totally off the left edge?
        if (tspan >= span)
            return false;

        angle1 = clipangle;
    }
    tspan = clipangle - angle2;
    if (tspan > 2 * clipangle)
    {
        tspan -= 2 * clipangle;

        // Totally off the left edge?
        if (tspan >= span)
            return false;

        angle2 = -clipangle;
    }


    // Find the first clippost
    //  that touches the source post
    //  (adjacent pixels are touching).
    angle1 = (angle1 + ANG90) >> ANGLETOFINESHIFT;
    angle2 = (angle2 + ANG90) >> ANGLETOFINESHIFT;
    sx1 = viewangletox[angle1];
    sx2 = viewangletox[angle2];

    // Does not cross a pixel.
    if (sx1 == sx2)
        return false;
    sx2--;

    start = solidsegs;
    while (start->last < sx2)
        start++;

    if (sx1 >= start->first
        && sx2 <= start->last)
    {
        // The clippost contains the new span.
        return false;
    }

    return true;
    */
}

//
// RenderBSPNode
// Renders all subsectors below a given node,
//  traversing subtree recursively.
// Just call with BSP root.

void R_RenderBSPNode(uint16_t nodenum)
{
    const node_t* bsp;
    int		side;

    // Found a subsector?
    if (nodenum & NF_SUBSECTOR)
    {
        if (nodenum == -1)
            R_Subsector(0);
        else
            R_Subsector(nodenum & (~NF_SUBSECTOR));
        return;
    }

    bsp = &currentlevel->nodes[nodenum];

    // Decide which side the view point is on.
    side = R_PointOnSide(viewx, viewy, bsp);

    // Recursively divide front space.
    R_RenderBSPNode(bsp->children[side]);

    if (columnsToFill <= 0)
        return;

    // Possibly divide back space.
    if (R_CheckBBox(bsp->bbox[side ^ 1]))
        R_RenderBSPNode(bsp->children[side ^ 1]);

}

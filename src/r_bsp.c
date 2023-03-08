#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include "r_local.h"
#include "tables.h"
#include "m_bbox.h"

#include "Map.h"

void DrawDebugLine(int x0, int y0, int x1, int y1);
void DrawMapDebugLine(int x0, int y0, int x1, int y1);

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

extern int viewangletox[FINEANGLES / 2];

#define FRONT_CLIP_PLANE 4

void VLine(int x, int y, int count, uint32_t colour);

int16_t floorClip[256];
int16_t ceilingClip[256];

void R_Subsector(uint16_t subSectorNum)
{
    mapsubsector_t* ssector = &Map.subsectors[subSectorNum];
    int i;
    int16_t segnum;

    //printf("Render subsector %d\n", subSectorNum);

    segnum = ssector->firstseg;

    angle_t negviewangle = -viewangle + ANG90;
    int16_t viewcos = finecosine[negviewangle >> ANGLETOFINESHIFT];
    int16_t viewsin = finesine[negviewangle >> ANGLETOFINESHIFT];

    for (i = 0; i < ssector->numsegs; i++, segnum++)
    {
        mapseg_t* seg = &Map.segs[segnum];
        maplinedef_t* linedef = &Map.lines[seg->linedef];
        int zoom = 3;

        //DrawMapDebugLine(
        //    320 + (Map.vertices[seg->v1].x - viewx) / zoom,
        //    200 - (Map.vertices[seg->v1].y - viewy) / zoom,
        //    321 + (Map.vertices[seg->v1].x - viewx) / zoom,
        //    201 - (Map.vertices[seg->v1].y - viewy) / zoom);
        //
        //DrawMapDebugLine(
        //    320 + (Map.vertices[seg->v1].x - viewx) / zoom,
        //    200 - (Map.vertices[seg->v1].y - viewy) / zoom,
        //    320 + (Map.vertices[seg->v2].x - viewx) / zoom,
        //    200 - (Map.vertices[seg->v2].y - viewy) / zoom);

        int16_t relvx1 = (Map.vertices[seg->v1].x - viewx);
        int16_t relvy1 = (Map.vertices[seg->v1].y - viewy);
        int16_t vx1 = ((relvx1 * viewcos) >> 8) - ((relvy1 * viewsin) >> 8);
        int16_t vy1 = ((relvx1 * viewsin) >> 8) + ((relvy1 * viewcos) >> 8);

        //if (vx1 > vy1)
        //{
        //    // Clipped to right side of the view space
        //    continue;
        //}

        int16_t relvx2 = (Map.vertices[seg->v2].x - viewx);
        int16_t relvy2 = (Map.vertices[seg->v2].y - viewy);
        int16_t vx2 = ((relvx2 * viewcos) >> 8) - ((relvy2 * viewsin) >> 8);
        int16_t vy2 = ((relvx2 * viewsin) >> 8) + ((relvy2 * viewcos) >> 8);

        int16_t sx1, sx2;
        const int maxsx = 255;

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

            vx1 += (FRONT_CLIP_PLANE - vy1) * (vx2 - vx1) / (vy2 - vy1);
            vy1 = FRONT_CLIP_PLANE;
        }
        else if (vy2 < FRONT_CLIP_PLANE)
        {
            if (vy1 < FRONT_CLIP_PLANE)
            {
                // Segment entirely behind the clip plane
                continue;
            }
            vx2 += (FRONT_CLIP_PLANE - vy2) * (vx1 - vx2) / (vy1 - vy2);
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
            vy2 = vx2 = vx1 + (vx2 - vx1) * (vx1 - vy1) / ((vy2 - vy1) - (vx2 - vx1));
            sx2 = maxsx;
        }
        else
        {
            sx2 = 128 + (128 * vx2) / vy2;
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
            vy1 = -vx1;
            sx1 = 0;
        }
        else
        {
            sx1 = 128 + (128 * vx1) / vy1;
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

        DrawMapDebugLine(
            320 + (vx1) / zoom,
            200 - (vy1) / zoom,
            320 + (vx2) / zoom,
            200 - (vy2) / zoom);

        int shade = 128 + (seg->angle >> 9);
        uint32_t colour = (shade) | (shade << 8) | (shade << 16);
        uint32_t floorColour = (240) | (200 << 8) | (150 << 16);
        uint32_t ceilingColour = (200) | (180 << 8) | (120 << 16);

        srand(segnum);
        colour = rand() | (rand() << 8) | (rand() << 16);

        if (linedef->sidenum[0] != -1)
        {
            mapsidedef_t* side = &Map.sides[linedef->sidenum[seg->side]];
            mapsector_t* sector = &Map.sectors[side->sector];

            srand(side->sector + 1024);
            floorColour = rand() | (rand() << 8) | (rand() << 16);
            ceilingColour = rand() | (rand() << 8) | (rand() << 16);

            int u1 = 120 - 128 * (sector->ceilingheight - viewz) / vy1;
            int u2 = 120 - 128 * (sector->ceilingheight - viewz) / vy2;
            int l1 = 120 - 128 * (sector->floorheight - viewz) / vy1;
            int l2 = 120 - 128 * (sector->floorheight - viewz) / vy2;

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
                            goto fullyclipped;
                        }
                    }

                    //if (u > ceilingClip[x])
                    //{
                    //    VLine(x, ceilingClip[x] + 1, u - ceilingClip[x] + 1, ceilingColour);
                    //}

                    if (side->midtexture[0] != '-')
                    {
                        ceilingClip[x] = floorClip[x] = 0;
                        if (y2 <= y1)
                            continue;
                        VLine(x, y1, y2 - y1, colour);
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

            if (seg->side == 0 && linedef->sidenum[1] != -1)
            {
                // Connected to another sector
                mapsidedef_t* backside = &Map.sides[linedef->sidenum[1]];
                mapsector_t* backsector = &Map.sectors[backside->sector];

                if (backsector->ceilingheight < sector->ceilingheight || backsector->floorheight > sector->floorheight)
                {
                    u1 = 120 - 128 * (backsector->ceilingheight - viewz) / vy1;
                    u2 = 120 - 128 * (backsector->ceilingheight - viewz) / vy2;
                    l1 = 120 - 128 * (backsector->floorheight - viewz) / vy1;
                    l2 = 120 - 128 * (backsector->floorheight - viewz) / vy2;

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
void R_Subsector2(uint16_t subSectorNum)
{
    //R_Subsector2(subSectorNum);
    mapsubsector_t* ssector = &Map.subsectors[subSectorNum];
    int i;
    int16_t segnum;

    //printf("Render subsector %d\n", subSectorNum);

    segnum = ssector->firstseg;

    for (i = 0; i < ssector->numsegs; i++, segnum++)
    {
        mapseg_t* seg = &Map.segs[segnum];
        int zoom = 3;

        DrawMapDebugLine(
            320 + (Map.vertices[seg->v1].x - viewx) / zoom,
            200 - (Map.vertices[seg->v1].y - viewy) / zoom,
            321 + (Map.vertices[seg->v1].x - viewx) / zoom,
            201 - (Map.vertices[seg->v1].y - viewy) / zoom);

        DrawMapDebugLine(
            320 + (Map.vertices[seg->v1].x - viewx) / zoom,
            200 - (Map.vertices[seg->v1].y - viewy) / zoom,
            320 + (Map.vertices[seg->v2].x - viewx) / zoom,
            200 - (Map.vertices[seg->v2].y - viewy) / zoom);

        int x1, y1, x2, y2;

        {
            angle_t angle = R_PointToAngle(Map.vertices[seg->v1].x, Map.vertices[seg->v1].y);
            angle = ((angle_t)(angle + ANG90 - viewangle)) >> ANGLETOFINESHIFT;
            int x = viewangletox[angle];
            int xdiff = (Map.vertices[seg->v1].x - viewx);
            int ydiff = (Map.vertices[seg->v1].y - viewy);
            float distance = sqrt(xdiff * xdiff + ydiff * ydiff);
            int height = distance > 0 ? (int)(10000 / distance) : 0;
            DrawDebugLine(x, 120 - height, x, 120 + height);
            x1 = x;
            y1 = 120 + height;
        }
        {
            angle_t angle = R_PointToAngle(Map.vertices[seg->v2].x, Map.vertices[seg->v2].y);
            angle = ((angle_t)(angle + ANG90 - viewangle)) >> ANGLETOFINESHIFT;
            int x = viewangletox[angle];
            int xdiff = (Map.vertices[seg->v2].x - viewx);
            int ydiff = (Map.vertices[seg->v2].y - viewy);
            float distance = sqrt(xdiff * xdiff + ydiff * ydiff);
            int height = distance > 0 ? (int)(10000 / distance) : 0;
            DrawDebugLine(x, 120 - height, x, 120 + height);
            x2 = x;
            y2 = 120 + height;
        }

        //DrawDebugLine(x1, y1, x2, y2);
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
    if (boxpos == 5)
        return true;

    x1 = bspcoord[checkcoord[boxpos][0]];
    y1 = bspcoord[checkcoord[boxpos][1]];
    x2 = bspcoord[checkcoord[boxpos][2]];
    y2 = bspcoord[checkcoord[boxpos][3]];

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
    mapnode_t* bsp;
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

    bsp = &Map.nodes[nodenum];

    // Decide which side the view point is on.
    side = R_PointOnSide(viewx, viewy, bsp);

    // Recursively divide front space.
    R_RenderBSPNode(bsp->children[side]);

    // Possibly divide back space.
    if (R_CheckBBox(bsp->bbox[side ^ 1]))
        R_RenderBSPNode(bsp->children[side ^ 1]);

}

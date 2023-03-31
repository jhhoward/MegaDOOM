#include "doomtypes.h"
#include "r_local.h"
#include "tables.h"
#include "m_bbox.h"
#include "r_defs.h"

#include "Map.h"

extern const uint8_t textureatlas[];
extern const walltexture_t walltextures[];
extern const flat_t flats[];

void TexturedLine(const uint8_t* texptr, int16_t x, int16_t y, int16_t count, int16_t u, int16_t v, int16_t step);


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
    int16_t first;
    int16_t last;
} cliprange_t;

#define MAXSEGS (VIEWPORT_WIDTH / 2 + 1)

// newend is one past the last valid seg
cliprange_t* newend;
cliprange_t	solidsegs[MAXSEGS];

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

#define FRONT_CLIP_PLANE 16
#define FAR_CLIP_PLANE 2000

void VLine(int x, int y, int count, uint8_t colour);

int16_t floorClip[VIEWPORT_WIDTH];
int16_t ceilingClip[VIEWPORT_WIDTH];
int16_t columnsToFill;

angle_t clipangle;
angle_t negviewangle;
int16_t viewcos;
int16_t viewsin;

#define USE_DIVIDE_INSTRUCTIONS 0

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


inline int32_t ScaleByDistanceX(int16_t x, int16_t z)
{
#if USE_DIVIDE_INSTRUCTIONS 
    return (x * VIEWPORT_HALF_WIDTH) / z;
#else
    while (z > 4096)
    {
        z >>= 1;
        x >>= 1;
    }
    return (x * distancescalex[z]) >> 10;
#endif
}

inline int32_t ScaleByDistanceY(int16_t x, int16_t z)
{
#if USE_DIVIDE_INSTRUCTIONS 
    return (x * VIEWPORT_WIDTH) / z;
#else
    while (z > 4096)
    {
        z >>= 1;
        x >>= 1;
    }
    return (x * distancescaley[z]);
#endif
}

// View space vertex positions
int16_t vs_x1, vs_y1;
int16_t vs_x2, vs_y2;

// Texture coordinates
int16_t vs_tx1, vs_tx2;
int16_t vs_ty1, vs_ty2;

// Screen space vertex positions
int16_t ss_x1, ss_x2;
int16_t ss_deltax;
int32_t ss_upper1_fx, ss_upper2_fx;
int32_t ss_lower1_fx, ss_lower2_fx;

const walltexture_t* draw_walltexture;
uint8_t draw_wallcolour;
uint8_t draw_floorcolour;
uint8_t draw_ceilingcolour;
int16_t draw_shading;
bool draw_fillfloor, draw_fillceiling, draw_fillwall;

void R_StoreWallRangeSimple(int16_t left, int16_t right);

void R_StoreWallRange(int16_t left, int16_t right)
{
    if (ss_x2 - ss_x1 <= 2 || ((ss_lower1_fx - ss_upper1_fx) < (6 << 8) && (ss_lower2_fx - ss_upper2_fx) < (6 << 8)))
    {
        R_StoreWallRangeSimple(left, right);
        return;
    }

    int16_t deltaupper_fx = (ss_upper2_fx - ss_upper1_fx) / ss_deltax;
    int16_t deltalower_fx = (ss_lower2_fx - ss_lower1_fx) / ss_deltax;
    int32_t upper_fx = ss_upper1_fx;
    int32_t lower_fx = ss_lower1_fx;
    int32_t deltatx_fx = ((vs_tx2 - vs_tx1) << 8) / ss_deltax;
    int32_t tx_fx = vs_tx1 << 8;

    const uint32_t* columns = draw_walltexture->columns[draw_shading];

    if (left > ss_x1)
    {
        int16_t diff = left - ss_x1;
        upper_fx += deltaupper_fx * diff;
        lower_fx += deltalower_fx * diff;
        tx_fx += deltatx_fx * diff;
    }

    for (int16_t x = left; x <= right; x++)
    {
        int16_t top = upper_fx >> 8;
        int16_t bottom = lower_fx >> 8;
        int16_t clipTop = ceilingClip[x];
        int16_t clipBottom = floorClip[x];
        int16_t ty, step;
        
        if (draw_fillwall)
        {
            ty = vs_ty1;
            int16_t scalediv = ((lower_fx - upper_fx) >> 8);
            if (scalediv == 0)
                scalediv = 1;
            step = ((vs_ty2 - vs_ty1) << 8) / scalediv;
            //step = ((vs_ty2 - vs_ty1) * reciprocal[scalediv]) >> 2;
        }


        if (clipTop < clipBottom)
        {
            if (top <= clipTop)
            {
                if (top != bottom && draw_fillwall)
                {
                    ty += (step * (clipTop - top)) >> 8;
                    //ty = vs_ty1 + ((clipTop + 1) - top) * (vs_ty2 - vs_ty1) / (bottom - top);
                    //ty = ClipValue(top, bottom, vs_ty1, vs_ty2, clipTop + 1);
                }
                top = clipTop + 1;
            }
            else
            {
                if (draw_fillceiling)
                {
                    if (top < clipBottom)
                    {
                        VLine(x, clipTop + 1, top - clipTop - 1, draw_ceilingcolour);
                    }
                    else
                    {
                        VLine(x, clipTop + 1, clipBottom - clipTop - 1, draw_ceilingcolour);
                        ceilingClip[x] = floorClip[x] = 0;
                        goto fullyclipped;
                    }
                }
            }

            if (bottom > clipBottom)
            {
                bottom = clipBottom;
            }
            else
            {
                if (draw_fillfloor)
                {
                    if (bottom > clipTop)
                    {
                        VLine(x, bottom, clipBottom - bottom, draw_floorcolour);
                    }
                    else
                    {
                        VLine(x, clipTop + 1, clipBottom - clipTop - 1, draw_floorcolour);
                        ceilingClip[x] = floorClip[x] = 0;
                        goto fullyclipped;
                    }
                }
            }

            if (top < bottom)
            {
                if (draw_fillwall)
                {
                    if (draw_fillceiling)
                    {
                        ceilingClip[x] = bottom - 1;
                    }
                    if (draw_fillfloor)
                    {
                        floorClip[x] = top;
                    }

                    int16_t texcoord = ((tx_fx >> 8) & (draw_walltexture->width - 1));
                    TexturedLine(textureatlas + columns[texcoord], x, top, bottom - top, texcoord, ty, step);
                }
                else
                {
                    ceilingClip[x] = top - 1;
                    floorClip[x] = bottom;
                }
            }
        }

        fullyclipped:
        upper_fx += deltaupper_fx;
        lower_fx += deltalower_fx;
        tx_fx += deltatx_fx;
    }
}

void R_StoreWallRangeSimple(int16_t left, int16_t right)
{
    int16_t deltaupper_fx = (ss_upper2_fx - ss_upper1_fx) / ss_deltax;
    int16_t deltalower_fx = (ss_lower2_fx - ss_lower1_fx) / ss_deltax;
    int32_t upper_fx = ss_upper1_fx;
    int32_t lower_fx = ss_lower1_fx;
    uint8_t wallcolour = draw_walltexture->colour[draw_shading];

    if (left > ss_x1)
    {
        int16_t diff = left - ss_x1;
        upper_fx += deltaupper_fx * diff;
        lower_fx += deltalower_fx * diff;
    }

    for (int16_t x = left; x <= right; x++)
    {
        int16_t top = upper_fx >> 8;
        int16_t bottom = lower_fx >> 8;
        int16_t clipTop = ceilingClip[x];
        int16_t clipBottom = floorClip[x];

        if (clipTop < clipBottom)
        {
            if (top <= clipTop)
            {
                top = clipTop + 1;
            }
            else
            {
                if (draw_fillceiling)
                {
                    if (top < clipBottom)
                    {
                        VLine(x, clipTop + 1, top - clipTop - 1, draw_ceilingcolour);
                    }
                    else
                    {
                        VLine(x, clipTop + 1, clipBottom - clipTop - 1, draw_ceilingcolour);
                        ceilingClip[x] = floorClip[x] = 0;
                        goto fullyclipped;
                    }
                }
            }

            if (bottom > clipBottom)
            {
                bottom = clipBottom;
            }
            else
            {
                if (draw_fillfloor)
                {
                    if (bottom > clipTop)
                    {
                        VLine(x, bottom, clipBottom - bottom, draw_floorcolour);
                    }
                    else
                    {
                        VLine(x, clipTop + 1, clipBottom - clipTop - 1, draw_floorcolour);
                        ceilingClip[x] = floorClip[x] = 0;
                        goto fullyclipped;
                    }
                }
            }

            if (top < bottom)
            {
                if (draw_fillwall)
                {
                    if (draw_fillceiling)
                    {
                        ceilingClip[x] = bottom - 1;
                    }
                    if (draw_fillfloor)
                    {
                        floorClip[x] = top;
                    }

                    VLine(x, top, bottom - top, wallcolour);
                }
                else
                {
                    ceilingClip[x] = top - 1;
                    floorClip[x] = bottom;
                }
            }
        }

    fullyclipped:
        upper_fx += deltaupper_fx;
        lower_fx += deltalower_fx;
    }
}

//
// R_ClipPassWallSegment
// Clips the given range of columns,
//  but does not includes it in the clip list.
// Does handle windows,
//  e.g. LineDefs with upper and lower texture.
//
void R_ClipPassWallSegment (int16_t first, int16_t last)
{
    cliprange_t* start;

    // Find the first range that touches the range
    //  (adjacent pixels are touching).
    start = solidsegs;
    while (start->last < first - 1)
        start++;

    if (first < start->first)
    {
        if (last < start->first - 1)
        {
            // Post is entirely visible (above start).
            R_StoreWallRange(first, last);
            return;
        }

        // There is a fragment above *start.
        R_StoreWallRange(first, start->first - 1);
    }

    // Bottom contained in start?
    if (last <= start->last)
        return;

    while (last >= (start + 1)->first - 1)
    {
        // There is a fragment between two posts.
        R_StoreWallRange(start->last + 1, (start + 1)->first - 1);
        start++;

        if (last <= start->last)
            return;
    }

    // There is a fragment after *next.
    R_StoreWallRange(start->last + 1, last);
}


//
// R_ClipSolidWallSegment
// Does handle solid walls,
//  e.g. single sided LineDefs (middle texture)
//  that entirely block the view.
// 
void R_ClipSolidWallSegment(int16_t first, int16_t last)
{
    cliprange_t* next;
    cliprange_t* start;

    // Find the first range that touches the range
    //  (adjacent pixels are touching).
    start = solidsegs;
    while (start->last < first - 1)
        start++;

    if (first < start->first)
    {
        if (last < start->first - 1)
        {
            // Post is entirely visible (above start),
            //  so insert a new clippost.
            R_StoreWallRange(first, last);
            next = newend;
            newend++;

            while (next != start)
            {
                *next = *(next - 1);
                next--;
            }
            next->first = first;
            next->last = last;
            return;
        }

        // There is a fragment above *start.
        R_StoreWallRange(first, start->first - 1);
        // Now adjust the clip size.
        start->first = first;
    }

    // Bottom contained in start?
    if (last <= start->last)
        return;

    next = start;
    while (last >= (next + 1)->first - 1)
    {
        // There is a fragment between two posts.
        R_StoreWallRange(next->last + 1, (next + 1)->first - 1);
        next++;

        if (last <= next->last)
        {
            // Bottom is contained in next.
            // Adjust the clip size.
            start->last = next->last;
            goto crunch;
        }
    }

    // There is a fragment after *next.
    R_StoreWallRange(next->last + 1, last);
    // Adjust the clip size.
    start->last = last;

    // Remove start+1 to next from the clip list,
    // because start now covers their area.
crunch:
    if (next == start)
    {
        // Post just extended past the bottom of one post.
        return;
    }

    while (next++ != newend)
    {
        // Remove a post.
        *++start = *next;
    }

    newend = start + 1;
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
        const line_t* linedef = seg->linedef;

        if (linedef->sidenum[0] == -1)
        {
            continue;
        }

        // Cheap back face culling check
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


        // Transform first vertex into view space
        int16_t relvx1 = (seg->v1->x - viewx);
        int16_t relvy1 = (seg->v1->y - viewy);
        vs_x1 = (relvx1 * viewcos - relvy1 * viewsin) >> 8;
        vs_y1 = (relvx1 * viewsin + relvy1 * viewcos) >> 8;

        if (vs_x1 > abs(vs_y1))
        {
            // Clipped to right side of the view space
            continue;
        }

        int16_t relvx2 = (seg->v2->x - viewx);
        int16_t relvy2 = (seg->v2->y - viewy);
        vs_x2 = (relvx2 * viewcos - relvy2 * viewsin) >> 8;
        vs_y2 = (relvx2 * viewsin + relvy2 * viewcos) >> 8;

        DEBUG_MAP_LINE(vs_x1, vs_y1, vs_x2, vs_y2, 0xff444444);

        if (vs_y1 > FAR_CLIP_PLANE && vs_y2 > FAR_CLIP_PLANE)
        {
            continue;
        }

        if (vs_x2 < -abs(vs_y2))
        {
            // Clipped to the left side of the view space
            continue;
        }

        vs_tx1 = 0;
        vs_tx2 = seg->length;

        if (vs_y1 < FRONT_CLIP_PLANE)
        {
            if (vs_y2 < FRONT_CLIP_PLANE)
            {
                // Segment entirely behind the clip plane
                continue;
            }

            vs_tx1 += (FRONT_CLIP_PLANE - vs_y1) * (vs_tx2 - vs_tx1) / (vs_y2 - vs_y1);
            vs_x1 += (FRONT_CLIP_PLANE - vs_y1) * (vs_x2 - vs_x1) / (vs_y2 - vs_y1);
            //tx1 = ClipValue(vy1, vy2, tx1, tx2, FRONT_CLIP_PLANE);
            //vx1 = ClipValue(vy1, vy2, vx1, vx2, FRONT_CLIP_PLANE);
            vs_y1 = FRONT_CLIP_PLANE;
        }
        else if (vs_y2 < FRONT_CLIP_PLANE)
        {
            if (vs_y1 < FRONT_CLIP_PLANE)
            {
                // Segment entirely behind the clip plane
                continue;
            }

            vs_tx2 = vs_tx1 + (FRONT_CLIP_PLANE - vs_y1) * (vs_tx2 - vs_tx1) / (vs_y2 - vs_y1);
            vs_x2 += (FRONT_CLIP_PLANE - vs_y2) * (vs_x1 - vs_x2) / (vs_y1 - vs_y2);
            //tx2 = ClipValue(vy2, vy1, tx2, tx1, FRONT_CLIP_PLANE);
            //vx2 = ClipValue(vy2, vy1, vx2, vx1, FRONT_CLIP_PLANE);
            vs_y2 = FRONT_CLIP_PLANE;
        }

        // Clip to the right FOV
        
        if (vs_x2 > vs_y2)
        {
            if (vs_x1 > vs_y1)
            {
                // Completely clipped
                continue;
            }
            // Right side needs clipping
            // x' = x0 + (x1 - x0) * ((x0 - y0) / ((y1 - y0) - (x1 - x0)))
            int16_t clippoint = vs_x1 + (vs_x2 - vs_x1) * (vs_x1 - vs_y1) / ((vs_y2 - vs_y1) - (vs_x2 - vs_x1));

            //if (vx1 == vx2)
            //{
            //    tx2 = ClipValue(vy2, vy1, tx2, tx1, clippoint);
            //}
            //else
            //{
            //    tx2 = ClipValue(vx2, vx1, tx2, tx1, clippoint);
            //}

            //if (vs_x2 != vs_x1)
            if(abs(vs_x2 - vs_x1) > abs(vs_y2 - vs_y1))
            {
                vs_tx2 = vs_tx1 + (clippoint - vs_x1) * (vs_tx2 - vs_tx1) / (vs_x2 - vs_x1);
            }
            else
            {
                vs_tx2 = vs_tx1 + (clippoint - vs_y1) * (vs_tx2 - vs_tx1) / (vs_y2 - vs_y1);
            }

            vs_y2 = vs_x2 = clippoint;

            //vy2 = vx2 = ClipFOVRight(vx1, vy1, vx2, vy2);
            ss_x2 = VIEWPORT_WIDTH - 1;
        }
        else
        {
            //sx2 = (VIEWPORT_WIDTH / 2) + ((VIEWPORT_WIDTH / 2) * vx2) / vy2;
            ss_x2 = VIEWPORT_HALF_WIDTH + ScaleByDistanceX(vs_x2, vs_y2);
        }

        // Clip to left FOV
        if (vs_x1 < -vs_y1)
        {
            if (vs_x2 < -vs_y2)
            {
                // Completely clipped
                continue;
            }
            // Left side needs clipping
            // x' = x0 + (x1 - x0) * ((x0 - y0) / ((y1 - y0) - (x1 - x0)))
            int16_t clippoint = vs_x1 + (vs_x2 - vs_x1) * (-vs_x1 - vs_y1) / ((vs_y2 - vs_y1) + (vs_x2 - vs_x1));
            //vx1 = vx1 + (((vx2 - vx1) * (-vx1 - vy1) * reciprocal[((vy2 - vy1) + (vx2 - vx1))]) >> 10);
            //if (vx1 == vx2)
            //{
            //    tx1 = ClipValue(vy2, vy1, tx2, tx1, clippoint);
            //}
            //else
            //{
            //    tx1 = ClipValue(vx2, vx1, tx2, tx1, clippoint);
            //}

//            if (vs_x2 != vs_x1)
            if (abs(vs_x2 - vs_x1) > abs(vs_y2 - vs_y1))
            {
                vs_tx1 += (clippoint - vs_x1) * (vs_tx2 - vs_tx1) / (vs_x2 - vs_x1);
            }
            else
            {
                vs_tx1 += (clippoint - vs_y1) * (vs_tx2 - vs_tx1) / (vs_y2 - vs_y1);
            }


            vs_x1 = clippoint;
            vs_y1 = -vs_x1;
            ss_x1 = 0;
        }
        else
        {
            //sx1 = (VIEWPORT_WIDTH / 2) + ((VIEWPORT_WIDTH / 2) * vx1) / vy1;
            ss_x1 = VIEWPORT_HALF_WIDTH + ScaleByDistanceX(vs_x1, vs_y1);
        }
        

        //if (sx1 < 0)
        //    sx1 = 0;
        //if (sx2 > maxsx)
        //    sx2 = maxsx;

        ss_deltax = ss_x2 - ss_x1;

        if (ss_deltax <= 0)
        {
            // Back face culled
            continue;
        }

        DEBUG_MAP_LINE(vs_x1, vs_y1, vs_x2, vs_y2, 0xffffffff);

//        int shade = 128 + (seg->angle >> 9);
//        uint8_t colour;
//        uint8_t floorColour = 0x11;
//        uint8_t ceilingColour = 0x88;
//
//        //srand(segnum);
//        //colour = (uint8_t)rand();
//        colour = (uint8_t)(segnum);

        vs_tx1 += seg->offset;
        vs_tx2 += seg->offset;

        const side_t* side = seg->sidedef;
        const sector_t* sector = side->sector;

        draw_floorcolour = flats[sector->floorpic].colour[sector->lightlevel];
        draw_ceilingcolour = flats[sector->ceilingpic].colour[sector->lightlevel];

        draw_shading = sector->lightlevel;
        if (draw_shading)
        {
            if (seg->angle < ANG45 || seg->angle >(ANG270 + ANG45) || (seg->angle > ANG90 + ANG45 && seg->angle < ANG180 + ANG45))
            {
                draw_shading--;
            }
        }

        vs_tx1 += side->textureoffset;
        vs_tx2 += side->textureoffset;

        const sector_t* backsector = linedef->backsector;

        if (side->midtexture)
        {
            draw_walltexture = &walltextures[side->midtexture];
            //draw_wallcolour = (uint8_t)(seg);
            draw_fillfloor = true;
            draw_fillwall = true;
            draw_fillceiling = true;

            vs_ty2 = sector->ceilingheight - sector->floorheight;

            if (linedef->flags & ML_DONTPEGBOTTOM)
            {
                vs_ty1 = (draw_walltexture->height - vs_ty2) & 127;
                vs_ty2 += vs_ty1;
            }
            else
            {
                vs_ty1 = 0;
            }

            // TODO: make ScaleByDistanceY() return a fixed value instead of shifting here
            ss_upper1_fx = VIEWPORT_HALF_HEIGHT_FX - ScaleByDistanceY(sector->ceilingheight - viewz, vs_y1);
            ss_upper2_fx = VIEWPORT_HALF_HEIGHT_FX - ScaleByDistanceY(sector->ceilingheight - viewz, vs_y2);
            ss_lower1_fx = VIEWPORT_HALF_HEIGHT_FX - ScaleByDistanceY(sector->floorheight - viewz, vs_y1);
            ss_lower2_fx = VIEWPORT_HALF_HEIGHT_FX - ScaleByDistanceY(sector->floorheight - viewz, vs_y2);

            R_ClipSolidWallSegment(ss_x1, ss_x2);
            //R_DrawSolidSeg();
        }
        else if(backsector)
        {
            bool hasupper = side->toptexture && backsector->ceilingheight < sector->ceilingheight;
            bool haslower = side->bottomtexture && backsector->floorheight > sector->floorheight;

            // Connected to another sector
            if (hasupper)
            {
                draw_walltexture = &walltextures[side->toptexture];
                //draw_wallcolour = walltextures[side->toptexture].colour[shading];
                //draw_wallcolour = (uint8_t)(seg + 50);

                draw_fillfloor = false;
                draw_fillwall = true;
                draw_fillceiling = true;

                ss_upper1_fx = (VIEWPORT_HALF_HEIGHT_FX - ScaleByDistanceY(sector->ceilingheight - viewz, vs_y1));
                ss_upper2_fx = (VIEWPORT_HALF_HEIGHT_FX - ScaleByDistanceY(sector->ceilingheight - viewz, vs_y2));
                ss_lower1_fx = (VIEWPORT_HALF_HEIGHT_FX - ScaleByDistanceY(backsector->ceilingheight - viewz, vs_y1));
                ss_lower2_fx = (VIEWPORT_HALF_HEIGHT_FX - ScaleByDistanceY(backsector->ceilingheight - viewz, vs_y2));

                vs_ty2 = sector->ceilingheight - backsector->ceilingheight;
                if (linedef->flags & ML_DONTPEGTOP)
                {
                    vs_ty1 = 0;
                }
                else
                {
                    vs_ty1 = (draw_walltexture->height - vs_ty2) & 127;
                    vs_ty2 += vs_ty1;
                }

                R_ClipPassWallSegment(ss_x1, ss_x2);
            }

            if (haslower)
            {
                draw_walltexture = &walltextures[side->bottomtexture];
                //draw_wallcolour = walltextures[side->bottomtexture].colour[shading];
                //draw_wallcolour = (uint8_t)(seg + 100);

                draw_fillfloor = true;
                draw_fillwall = true;
                draw_fillceiling = false;
                
                vs_ty2 = backsector->floorheight - sector->floorheight;

                if (linedef->flags & ML_DONTPEGBOTTOM)
                {
                    vs_ty1 = 0;
                }
                else
                {
                    vs_ty1 = (draw_walltexture->height - vs_ty2) & 127;
                    vs_ty2 += vs_ty1;
                }
                

                ss_upper1_fx = (VIEWPORT_HALF_HEIGHT_FX - ScaleByDistanceY(backsector->floorheight - viewz, vs_y1));
                ss_upper2_fx = (VIEWPORT_HALF_HEIGHT_FX - ScaleByDistanceY(backsector->floorheight - viewz, vs_y2));
                ss_lower1_fx = (VIEWPORT_HALF_HEIGHT_FX - ScaleByDistanceY(sector->floorheight - viewz, vs_y1));
                ss_lower2_fx = (VIEWPORT_HALF_HEIGHT_FX - ScaleByDistanceY(sector->floorheight - viewz, vs_y2));
                R_ClipPassWallSegment(ss_x1, ss_x2);
            }

            if (!hasupper || !haslower)
            {
                draw_fillfloor = !haslower;
                draw_fillceiling = !hasupper;
                draw_fillwall = false;

                ss_upper1_fx = VIEWPORT_HALF_HEIGHT_FX - ScaleByDistanceY(sector->ceilingheight - viewz, vs_y1);
                ss_upper2_fx = VIEWPORT_HALF_HEIGHT_FX - ScaleByDistanceY(sector->ceilingheight - viewz, vs_y2);
                ss_lower1_fx = VIEWPORT_HALF_HEIGHT_FX - ScaleByDistanceY(sector->floorheight - viewz, vs_y1);
                ss_lower2_fx = VIEWPORT_HALF_HEIGHT_FX - ScaleByDistanceY(sector->floorheight - viewz, vs_y2);
                R_ClipPassWallSegment(ss_x1, ss_x2);
            }
        }


#if 0
        quadparams.floorColour = flats[sector->floorpic].colour[sector->lightlevel];
        quadparams.ceilingColour = flats[sector->ceilingpic].colour[sector->lightlevel];
        quadparams.fillFloor = true;
        quadparams.fillCeiling = true;

        quadparams.upper1 = VIEWPORT_HALF_HEIGHT - ScaleByDistanceY(sector->ceilingheight - viewz, vs_y1);
        quadparams.upper2 = VIEWPORT_HALF_HEIGHT - ScaleByDistanceY(sector->ceilingheight - viewz, vs_y2);
        quadparams.lower1 = VIEWPORT_HALF_HEIGHT - ScaleByDistanceY(sector->floorheight - viewz, vs_y1);
        quadparams.lower2 = VIEWPORT_HALF_HEIGHT - ScaleByDistanceY(sector->floorheight - viewz, vs_y2);

        quadparams.tx1 = vs_tx1;
        quadparams.tx2 = vs_tx2;
        quadparams.sx1 = ss_x1;
        quadparams.sx2 = ss_x2;
        quadparams.ty1 = 0;
        quadparams.ty2 = sector->ceilingheight - sector->floorheight;
        quadparams.texture = side->midtexture ? &walltextures[side->midtexture] : NULL;
        if (quadparams.texture)
            quadparams.lightingShift = shading ? quadparams.texture->width : 0;

        R_TextureQuad(&quadparams);

        if (seg->linedef->backsector)
        {
            // Connected to another sector
            const sector_t* backsector = seg->linedef->backsector;


            if (side->toptexture && backsector->ceilingheight < sector->ceilingheight)
            {
                texturequadparams_t upperquadparams;
                upperquadparams.fillFloor = false;
                upperquadparams.fillCeiling = true;
                upperquadparams.lower1 = VIEWPORT_HALF_HEIGHT - ScaleByDistanceY(backsector->ceilingheight - viewz, vs_y1);
                upperquadparams.lower2 = VIEWPORT_HALF_HEIGHT - ScaleByDistanceY(backsector->ceilingheight - viewz, vs_y2);
                upperquadparams.upper1 = quadparams.upper1;
                upperquadparams.upper2 = quadparams.upper2;
                upperquadparams.texture = &walltextures[side->toptexture];
                upperquadparams.tx1 = vs_tx1;
                upperquadparams.tx2 = vs_tx2;
                upperquadparams.sx1 = ss_x1;
                upperquadparams.sx2 = ss_x2;
                upperquadparams.ty2 = sector->ceilingheight - backsector->ceilingheight;
                upperquadparams.lightingShift = shading ? upperquadparams.texture->width : 0;

                if (linedef->flags & ML_DONTPEGTOP)
                {
                    upperquadparams.ty1 = 0;
                }
                else
                {
                    upperquadparams.ty1 = (128 - upperquadparams.ty2) & 127;
                    upperquadparams.ty2 += upperquadparams.ty1;
                }

                R_TextureQuad(&upperquadparams);
            }
            if (side->bottomtexture && backsector->floorheight > sector->floorheight)
            {
                texturequadparams_t lowerquadparams;
                lowerquadparams.fillFloor = true;
                lowerquadparams.fillCeiling = false;
                lowerquadparams.upper1 = VIEWPORT_HALF_HEIGHT - ScaleByDistanceY(backsector->floorheight - viewz, vs_y1);
                lowerquadparams.upper2 = VIEWPORT_HALF_HEIGHT - ScaleByDistanceY(backsector->floorheight - viewz, vs_y2);
                lowerquadparams.lower1 = quadparams.lower1;
                lowerquadparams.lower2 = quadparams.lower2;
                lowerquadparams.texture = &walltextures[side->bottomtexture];
                lowerquadparams.tx1 = vs_tx1;
                lowerquadparams.tx2 = vs_tx2;
                lowerquadparams.sx1 = ss_x1;
                lowerquadparams.sx2 = ss_x2;
                lowerquadparams.ty1 = 0;
                lowerquadparams.ty2 = backsector->floorheight - sector->floorheight;
                lowerquadparams.lightingShift = shading ? lowerquadparams.texture->width : 0;

                R_TextureQuad(&lowerquadparams);
            }


        }
#endif
    }

}

//
// R_ClearClipSegs
//
void R_ClearClipSegs(void)
{
    solidsegs[0].first = -0x7fff;
    solidsegs[0].last = -1;
    solidsegs[1].first = VIEWPORT_WIDTH;
    solidsegs[1].last = 0x7fff;
    newend = solidsegs + 2;
}


bool R_CheckBBox(const int16_t* bspcoord)
{
    int16_t			boxx;
    int16_t			boxy;
    int16_t			boxpos;

    int16_t		x1;
    int16_t		y1;
    int16_t		x2;
    int16_t		y2;

    angle_t		angle1;
    angle_t		angle2;
    angle_t		span;
    angle_t		tspan;

    cliprange_t* start;

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

    int16_t vy1 = (x1 * viewsin + y1 * viewcos) >> 8;
    int16_t vy2 = (x2 * viewsin + y2 * viewcos) >> 8;

    // Check if box is behind the camera
    //if (vy1 < FRONT_CLIP_PLANE && vy2 < FRONT_CLIP_PLANE)
    if (vy1 < 0 && vy2 < 0)
            return false;

    int16_t vx1 = (x1 * viewcos - y1 * viewsin) >> 8;
    int16_t vx2 = (x2 * viewcos - y2 * viewsin) >> 8;

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

    int16_t sx1, sx2;

    if (vy1 < FRONT_CLIP_PLANE)
    {
        sx1 = 0;
        //if (vx1 < 0)
        //    sx1 = 0;
        //else
        //    sx1 = VIEWPORT_WIDTH - 1;
    }
    else
    {
        sx1 = VIEWPORT_HALF_WIDTH + ScaleByDistanceX(vx1, vy1);
    }

    if (vy2 < FRONT_CLIP_PLANE)
    {
        //if (vx2 < 0)
        //{
        //    sx2 = 0;
        //}
        //else
        {
            sx2 = VIEWPORT_WIDTH - 1;
        }
    }
    else
    {
        sx2 = VIEWPORT_HALF_WIDTH + ScaleByDistanceX(vx2, vy2);
    }

    //if (sx1 > sx2)
    //{
    //    int32_t temp = sx2;
    //    sx2 = sx1;
    //    sx1 = temp;
    //}

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
        if (nodenum == 0xffff)
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

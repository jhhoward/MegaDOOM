#include "doomtypes.h"
#include "Map.h"
#include "tables.h"
#include "i_video.h"
#include "r_defs.h"
#include "r_stat.h"
#include "r_local.h"

int16_t viewx, viewy;
int16_t viewz = 64;
angle_t viewangle;

const map_t* currentlevel;

void R_RenderView()
{
	negviewangle = -viewangle + ANG90;
	viewcos = finecosine[negviewangle >> ANGLETOFINESHIFT];
	viewsin = finesine[negviewangle >> ANGLETOFINESHIFT];
	clipangle = viewangle + ANG45;

	for (int n = 0; n < VIEWPORT_WIDTH; n++)
	{
		floorClip[n] = VIEWPORT_HEIGHT;
		ceilingClip[n] = -1;
	}

	columnsToFill = VIEWPORT_WIDTH;
	R_RenderBSPNode(currentlevel->rootnode);
}


//
// R_PointOnSide
// Traverse BSP (sub) tree,
//  check point against partition plane.
// Returns side 0 (front) or 1 (back).
//
int R_PointOnSide(int16_t x, int16_t y, const node_t* node)
{
    int16_t	dx;
    int16_t	dy;
    int32_t	left;
    int32_t	right;

    if (!node->dx)
    {
        if (x <= node->x)
            return node->dy > 0;

        return node->dy < 0;
    }
    if (!node->dy)
    {
        if (y <= node->y)
            return node->dx < 0;

        return node->dx > 0;
    }

    dx = (x - node->x);
    dy = (y - node->y);


    // Try to quickly decide by looking at sign bits.
    if ((node->dy ^ node->dx ^ dx ^ dy) & 0x8000)
    {
        if ((node->dy ^ dx) & 0x8000)
        {
            // (left is negative)
            return 1;
        }
        return 0;
    }


    //left = FixedMul(node->dy >> FRACBITS, dx);
    //right = FixedMul(dy, node->dx >> FRACBITS);
    left = node->dy * dx;
    right = node->dx * dy;

    if (right < left)
    {
        // front side
        return 0;
    }
    // back side
    return 1;
}

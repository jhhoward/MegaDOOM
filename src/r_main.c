#include "doomtypes.h"
#include "Map.h"
#include "tables.h"
#include "i_video.h"
#include "r_defs.h"

#define FRACBITS		8
#define FRACUNIT		(1<<FRACBITS)

// Fineangles in the SCREENWIDTH wide window.
#define FIELDOFVIEW		2048	

int16_t viewx, viewy;
int16_t viewz = 64;
angle_t viewangle;

int			centerx;
int			centery;

int32_t			centerxfrac;
int32_t			centeryfrac;
int32_t			projection;

//
// precalculated math tables
//
angle_t			clipangle;

// The viewangletox[viewangle + FINEANGLES/4] lookup
// maps the visible view angles to screen X coordinates,
// flattening the arc to a flat projection plane.
// There will be many angles mapped to the same X. 
int			viewangletox[FINEANGLES / 2];

// The xtoviewangleangle[] table maps a screen pixel
// to the lowest viewangle that maps back to x ranges
// from clipangle to -clipangle.
angle_t			xtoviewangle[SCREENWIDTH + 1];

int		viewwidth = 256;
int		viewheight = 224;

map_t* currentlevel;

int32_t
FixedMul
(int32_t	a,
	int32_t	b)
{
	return ((int64_t)a * (int64_t)b) >> FRACBITS;
}

int32_t FixedDiv(int32_t a, int32_t b)
{
	//if ((abs(a) >> 14) >= abs(b))
	//{
	//	return (a ^ b) < 0 ? SHORT_MIN : SHORT_MAX;
	//}
	//else
	{
		int64_t result;

		result = ((int64_t)a << FRACBITS) / b;

		return (int32_t)result;
	}
}

//
// R_InitTextureMapping
//
void R_InitTextureMapping(void)
{
	int			i;
	int			x;
	int			t;
	int			focallength;

	// Use tangent table to generate viewangletox:
	//  viewangletox will give the next greatest x
	//  after the view angle.
	//
	// Calc focallength
	//  so FIELDOFVIEW angles covers SCREENWIDTH.
	focallength = FixedDiv(centerxfrac,
		finetangent[FINEANGLES / 4 + FIELDOFVIEW / 2]);

	for (i = 0; i < FINEANGLES / 2; i++)
	{
		if (finetangent[i] > FRACUNIT * 2)
			t = -1;
		else if (finetangent[i] < -FRACUNIT * 2)
			t = viewwidth + 1;
		else
		{
			t = FixedMul(finetangent[i], focallength);
			t = (centerxfrac - t + FRACUNIT - 1) >> FRACBITS;

			if (t < -1)
				t = -1;
			else if (t > viewwidth + 1)
				t = viewwidth + 1;
		}
		viewangletox[i] = t;
	}

	// Scan viewangletox[] to generate xtoviewangle[]:
	//  xtoviewangle will give the smallest view angle
	//  that maps to x.	
	for (x = 0; x <= viewwidth; x++)
	{
		i = 0;
		while (viewangletox[i] > x)
			i++;
		xtoviewangle[x] = (i << ANGLETOFINESHIFT) - ANG90;
	}

	// Take out the fencepost cases from viewangletox.
	for (i = 0; i < FINEANGLES / 2; i++)
	{
		t = FixedMul(finetangent[i], focallength);
		t = centerx - t;

		if (viewangletox[i] == -1)
			viewangletox[i] = 0;
		else if (viewangletox[i] == viewwidth + 1)
			viewangletox[i] = viewwidth;
	}

	clipangle = xtoviewangle[0];
}


//
// R_PointOnSide
// Traverse BSP (sub) tree,
//  check point against partition plane.
// Returns side 0 (front) or 1 (back).
//
int R_PointOnSide(int16_t x, int16_t y, node_t* node)
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

//
// R_PointToAngle
// To get a global angle from cartesian coordinates,
//  the coordinates are flipped until they are in
//  the first octant of the coordinate system, then
//  the y (<=x) is scaled and divided by x to get a
//  tangent (slope) value which is looked up in the
//  tantoangle[] table.
//
angle_t
R_PointToAngle
(int16_t	x,
	int16_t	y)
{
	x -= viewx;
	y -= viewy;

	if ((!x) && (!y))
		return 0;

	if (x >= 0)
	{
		// x >=0
		if (y >= 0)
		{
			// y>= 0

			if (x > y)
			{
				// octant 0
				return tantoangle[SlopeDiv(y, x)];
			}
			else
			{
				// octant 1
				return ANG90 - 1 - tantoangle[SlopeDiv(x, y)];
			}
		}
		else
		{
			// y<0
			y = -y;

			if (x > y)
			{
				// octant 8
				return -tantoangle[SlopeDiv(y, x)];
			}
			else
			{
				// octant 7
				return ANG270 + tantoangle[SlopeDiv(x, y)];
			}
		}
	}
	else
	{
		// x<0
		x = -x;

		if (y >= 0)
		{
			// y>= 0
			if (x > y)
			{
				// octant 3
				return ANG180 - 1 - tantoangle[SlopeDiv(y, x)];
			}
			else
			{
				// octant 2
				return ANG90 + tantoangle[SlopeDiv(x, y)];
			}
		}
		else
		{
			// y<0
			y = -y;

			if (x > y)
			{
				// octant 4
				return ANG180 + tantoangle[SlopeDiv(y, x)];
			}
			else
			{
				// octant 5
				return ANG270 - 1 - tantoangle[SlopeDiv(x, y)];
			}
		}
	}
	return 0;
}

//
// R_ExecuteSetViewSize
//
void R_ExecuteSetViewSize(void)
{
	centery = viewheight / 2;
	centerx = viewwidth / 2;
	centerxfrac = centerx << FRACBITS;
	centeryfrac = centery << FRACBITS;
	projection = centerxfrac;

	R_InitTextureMapping();
}

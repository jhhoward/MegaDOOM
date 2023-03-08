//
// Copyright(C) 1993-1996 Id Software, Inc.
// Copyright(C) 2005-2014 Simon Howard
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// DESCRIPTION:
//	Lookup tables.
//	Do not try to look them up :-).
//	In the order of appearance: 
//
//	int finetangent[4096]	- Tangens LUT.
//	 Should work with BAM fairly well (12 of 16bit,
//      effectively, by shifting).
//
//	int finesine[10240]		- Sine lookup.
//	 Guess what, serves as cosine, too.
//	 Remarkable thing is, how to use BAMs with this? 
//
//	int tantoangle[2049]	- ArcTan LUT,
//	  maps tan(angle) to angle fast. Gotta search.
//	
//    

#include <stdint.h>
#include <math.h>
#include "tables.h"

#define FRACBITS		8
#define FRACUNIT		(1<<FRACBITS)

int16_t finetangent[4096];
int16_t finesine[10240];
int16_t* finecosine = &finesine[FINEANGLES / 4];
angle_t tantoangle[SLOPERANGE + 1];

#define PI 3.141592654f

void R_InitTables(void)
{
    int	i;
    int16_t	t;
    float	f;
    //
    // slope (tangent) to angle lookup
    //
    for (i = 0; i <= SLOPERANGE; i++)
    {
        f = atan((float)i / SLOPERANGE) / (3.141592657 * 2);
        t = 0xffff * f;
        tantoangle[i] = t;
    }

    float	a;
    float	fv;

    // viewangle tangent table
    for (i = 0; i < FINEANGLES / 2; i++)
    {
        a = (i - FINEANGLES / 4 + 0.5) * PI * 2 / FINEANGLES;
        fv = FRACUNIT * tan(a);
        t = fv;
        // t = (int)(t / 256) * 256;
        finetangent[i] = t;
    }

    // finesine table
    for (i = 0; i < 5 * FINEANGLES / 4; i++)
    {
        // OPTIMIZE: mirror...
        a = (i + 0.5) * PI * 2 / FINEANGLES;
        t = FRACUNIT * sin(a);
        //t = (int)(t / 256) * 256;
        finesine[i] = t;
    }
}

// to get a global angle from cartesian coordinates, the coordinates are
// flipped until they are in the first octant of the coordinate system, then
// the y (<=x) is scaled and divided by x to get a tangent (slope) value
// which is looked up in the tantoangle[] table.  The +1 size is to handle
// the case when x==y without additional checking.

int SlopeDiv(uint16_t num, uint16_t den)
{
    /*if (den > 0)
    {
        float ratio = (float)num / den;
        int index = (int)(ratio * SLOPERANGE);
        return index;
    }*/
    
    unsigned ans;

//    if (den < 256)
    if (den < 32)
    {
        return SLOPERANGE;
    }
    else
    {
//        ans = (num << 3) / (den >> 8);
        ans = ((uint32_t) num << 11) / (den);

        if (ans <= SLOPERANGE)
        {
            return ans;
        }
        else
        {
            return SLOPERANGE;
        }
    }
    
}


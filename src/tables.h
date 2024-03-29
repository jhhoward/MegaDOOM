//
// Copyright(C) 1993-1996 Id Software, Inc.
// Copyright(C) 1993-2008 Raven Software
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


#ifndef __TABLES__
#define __TABLES__

#include "doomtype.h"

#include "m_fixed.h"
	
#define FINEANGLES		8192
#define FINEMASK		(FINEANGLES-1)


// 0x100000000 to 0x2000
#define ANGLETOFINESHIFT	3 // 19

// Effective size is 10240.
extern const fixed16_t finesine[5*FINEANGLES/4];

// Re-use data, is just PI/2 pahse shift.
extern const fixed16_t *finecosine;


// Effective size is 4096.
extern const fixed_t finetangent[FINEANGLES/2];

extern const ufixed16_t scaledividetable[1024];

// Gamma correction tables.
//extern const byte gammatable[5][256];

// Binary Angle Measument, BAM.

#define ANG45           0x2000 // 0000
#define ANG90           0x4000 // 0000
#define ANG180          0x8000 // 0000
#define ANG270          0xc000 // 0000
#define ANG_MAX         0xffff // ffff

#define ANG1            (ANG45 / 45)
#define ANG60           (ANG180 / 3)

#define SLOPERANGE		2048
#define SLOPEBITS		11
#define DBITS			(FRACBITS-SLOPEBITS)

#define ANGLE_LOOKUP_TABLE_DIMENSION_BITS 8
#define ANGLE_LOOKUP_TABLE_DIMENSION (1 << ANGLE_LOOKUP_TABLE_DIMENSION_BITS)

typedef uint16_t angle_t;


// Effective size is 2049;
// The +1 size is to handle the case when x==y
//  without additional checking.
extern const angle_t tantoangle[SLOPERANGE+1];


// Utility function,
//  called by R_PointToAngle.
int SlopeDiv(unsigned int num, unsigned int den);

angle_t VectorToAngle(uint16_t x, uint16_t y);


#endif


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
//	Refresh/render internal state variables (global).
//


#ifndef __R_STATE__
#define __R_STATE__

#include "doomtypes.h"
#include "tables.h"
#include "r_defs.h"

//
// POV data.
//
extern int16_t		viewx;
extern int16_t		viewy;
extern int16_t		viewz;

extern angle_t		viewangle;

extern int16_t floorClip[VIEWPORT_WIDTH];
extern int16_t ceilingClip[VIEWPORT_WIDTH];
extern int16_t columnsToFill;

extern angle_t clipangle;
extern angle_t negviewangle;
extern int16_t viewcos;
extern int16_t viewsin;

#endif
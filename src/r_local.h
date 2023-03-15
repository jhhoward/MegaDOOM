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
// R_local.h

#ifndef __R_LOCAL__
#define __R_LOCAL__

#include "doomtypes.h"
#include "DoomData.h"
#include "r_stat.h"
#include "r_defs.h"

int R_PointOnSide(int16_t x, int16_t y, node_t* node);
void R_RenderBSPNode(uint16_t nodenum);
angle_t R_PointToAngle(int16_t x, int16_t y);

void R_ExecuteSetViewSize(void);

extern map_t* currentlevel;

#endif

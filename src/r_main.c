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
//	Rendering main loop and setup functions,
//	 utility functions (BSP, geometry, trigonometry).
//	See tables.c, too.
//





//#include <stdlib.h>
//#include <math.h>

#include "doomdata.h"
#include "doomdef.h"
//#include "d_loop.h"

#include "m_bbox.h"
//#include "m_menu.h"

#include "r_local.h"
#include "r_sky.h"





// Fineangles in the SCREENWIDTH wide window.
#define FIELDOFVIEW		2048	

const map_t* map;

int			viewangleoffset;

// increment every time a check is made
int			validcount = 1;		


//lighttable_t*		fixedcolormap;
//extern lighttable_t**	walllights;

int16_t			centerx;
int16_t			centery;

fixed_t			centerxfrac;
fixed_t			centeryfrac;
int16_t			projection;

// just for profiling purposes
int			framecount;	

int			sscount;
int			linecount;
int			loopcount;

int16_t			viewx;
int16_t			viewy;
int16_t			viewz;

angle_t			viewangle;

fixed_t			viewcos;
fixed_t			viewsin;

//player_t*		viewplayer;

// 0 = high, 1 = low
int			detailshift;	

//
// precalculated math tables
//
angle_t			clipangle;

// The viewangletox[viewangle + FINEANGLES/4] lookup
// maps the visible view angles to screen X coordinates,
// flattening the arc to a flat projection plane.
// There will be many angles mapped to the same X. 
int16_t			viewangletox[FINEANGLES/2];

// The xtoviewangleangle[] table maps a screen pixel
// to the lowest viewangle that maps back to x ranges
// from clipangle to -clipangle.
angle_t			xtoviewangle[SCREENWIDTH+1];

//lighttable_t*		scalelight[LIGHTLEVELS][MAXLIGHTSCALE];
//lighttable_t*		scalelightfixed[MAXLIGHTSCALE];
//lighttable_t*		zlight[LIGHTLEVELS][MAXLIGHTZ];

// bumped light from gun blasts
int			extralight;			



void (*colfunc) (void);
void (*basecolfunc) (void);
void (*fuzzcolfunc) (void);
void (*transcolfunc) (void);
void (*spanfunc) (void);



//
// R_AddPointToBox
// Expand a given bbox
// so that it encloses a given point.
//
void
R_AddPointToBox
( int		x,
  int		y,
  fixed_t*	box )
{
    if (x< box[BOXLEFT])
	box[BOXLEFT] = x;
    if (x> box[BOXRIGHT])
	box[BOXRIGHT] = x;
    if (y< box[BOXBOTTOM])
	box[BOXBOTTOM] = y;
    if (y> box[BOXTOP])
	box[BOXTOP] = y;
}


//
// R_PointOnSide
// Traverse BSP (sub) tree,
//  check point against partition plane.
// Returns side 0 (front) or 1 (back).
//
int
R_PointOnSide
( int16_t	x,
  int16_t	y,
  const node_t*	node )
{
    int16_t	dx;
    int16_t	dy;
    fixed_t	left;
    fixed_t	right;
	
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

    left = node->dy * dx;
    right = dy * node->dx;
	
    if (right < left)
    {
	// front side
	return 0;
    }
    // back side
    return 1;			
}


int
R_PointOnSegSide
( int16_t	x,
  int16_t	y,
  const seg_t*	line )
{
    int16_t	lx;
    int16_t	ly;
    int16_t	ldx;
    int16_t	ldy;
    int16_t	dx;
    int16_t	dy;
    fixed_t	left;
    fixed_t	right;
	
    lx = line->v1->x;
    ly = line->v1->y;
	
    ldx = line->v2->x - lx;
    ldy = line->v2->y - ly;
	
    if (!ldx)
    {
	if (x <= lx)
	    return ldy > 0;
	
	return ldy < 0;
    }
    if (!ldy)
    {
	if (y <= ly)
	    return ldx < 0;
	
	return ldx > 0;
    }
	
    dx = (x - lx);
    dy = (y - ly);
	
    // Try to quickly decide by looking at sign bits.
    if ( (ldy ^ ldx ^ dx ^ dy)&0x8000 )
    {
	if  ( (ldy ^ dx) & 0x8000 )
	{
	    // (left is negative)
	    return 1;
	}
	return 0;
    }

    //left = FixedMul ( ldy>>FRACBITS , dx );
    //right = FixedMul ( dy , ldx>>FRACBITS );
    left = ldy * dx;
    right = dy * ldx;
	
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



#if 1
angle_t
R_PointToAngle
( int16_t	x,
  int16_t	y )
{
    x -= viewx;
    y -= viewy;

    if (x >= 0)
    {
        if (y >= 0)
        {
            return VectorToAngle(x, y);
        }
        else
        {
            return ANG_MAX - VectorToAngle(x, -y);
        }
    }
    else
    {
        if (y >= 0)
        {
            return ANG180 - VectorToAngle(-x, y);
        }
        else
        {
            return ANG180 + VectorToAngle(-x, -y);
        }
    }
}

#else

angle_t
R_PointToAngle
( fixed_t	x,
  fixed_t	y )
{	
    x -= viewx;
    y -= viewy;
    
    if ( (!x) && (!y) )
	return 0;

    if (x>= 0)
    {
	// x >=0
	if (y>= 0)
	{
	    // y>= 0

	    if (x>y)
	    {
		// octant 0
		return tantoangle[ SlopeDiv(y,x)];
	    }
	    else
	    {
		// octant 1
		return ANG90-1-tantoangle[ SlopeDiv(x,y)];
	    }
	}
	else
	{
	    // y<0
	    y = -y;

	    if (x>y)
	    {
		// octant 8
            return ANG_MAX -tantoangle[SlopeDiv(y, x)];
	    }
	    else
	    {
		// octant 7
		return ANG270+tantoangle[ SlopeDiv(x,y)];
	    }
	}
    }
    else
    {
	// x<0
	x = -x;

	if (y>= 0)
	{
	    // y>= 0
	    if (x>y)
	    {
		// octant 3
		return ANG180-1-tantoangle[ SlopeDiv(y,x)];
	    }
	    else
	    {
		// octant 2
		return ANG90+ tantoangle[ SlopeDiv(x,y)];
	    }
	}
	else
	{
	    // y<0
	    y = -y;

	    if (x>y)
	    {
		// octant 4
		return ANG180+tantoangle[ SlopeDiv(y,x)];
	    }
	    else
	    {
		 // octant 5
		return ANG270-1-tantoangle[ SlopeDiv(x,y)];
	    }
	}
    }
    return 0;
}
#endif

angle_t
R_PointToAngle2
( int16_t	x1,
  int16_t	y1,
  int16_t	x2,
  int16_t	y2 )
{	
    viewx = x1;
    viewy = y1;
    
    return R_PointToAngle (x2, y2);
}

#ifndef abs
#define abs(x) ((x) < 0 ? (-(x)) : (x))
#endif

int16_t
R_PointToDist
( int16_t	x,
  int16_t	y )
{
    angle_t	angle;
    int16_t	dx;
    int16_t	dy;
    int16_t	temp;
    int16_t	dist;
    int16_t frac;
	
    dx = abs(x - viewx);
    dy = abs(y - viewy);

    if (dy>dx)
    {
	temp = dx;
	dx = dy;
	dy = temp;
    }

    // Fix crashes in udm1.wad
    if (dx != 0)
    {
        frac = (dy << FRACBITS) / dx;
    }
    else
    {
        frac = 0;
    }

    angle = tantoangle[frac >> DBITS] + ANG90;

    // use as cosine
    dist = (dx << FRACBITS) / finesine[angle >> ANGLETOFINESHIFT];

    return dist;

}




//
// R_InitPointToAngle
//
void R_InitPointToAngle (void)
{
    // UNUSED - now getting from tables.c
#if 0
    int	i;
    long	t;
    float	f;
//
// slope (tangent) to angle lookup
//
    for (i=0 ; i<=SLOPERANGE ; i++)
    {
	f = atan( (float)i/SLOPERANGE )/(3.141592657*2);
	t = 0xffffffff*f;
	tantoangle[i] = t;
    }
#endif
}


//
// R_ScaleFromGlobalAngle
// Returns the texture mapping scale
//  for the current line (horizontal span)
//  at the given angle.
// rw_distance must be calculated first.
//
fixed_t R_ScaleFromGlobalAngle (angle_t visangle)
{
    angle_t		anglea;
    angle_t		angleb;
    fixed16_t	sinea;
    fixed16_t	sineb;
    int32_t		num;
    int32_t		den;

    // UNUSED
#if 0
{
    fixed_t		dist;
    fixed_t		z;
    fixed_t		sinv;
    fixed_t		cosv;
	
    sinv = finesine[(visangle-rw_normalangle)>>ANGLETOFINESHIFT];	
    dist = FixedDiv (rw_distance, sinv);
    cosv = finecosine[(viewangle-visangle)>>ANGLETOFINESHIFT];
    z = abs(FixedMul (dist, cosv));
    scale = FixedDiv(projection, z);
    return scale;
}
#endif

    anglea = ANG90 + (visangle-viewangle);
    angleb = ANG90 + (visangle-rw_normalangle);

    // both sines are allways positive
    sinea = finesine[anglea>>ANGLETOFINESHIFT];	
    sineb = finesine[angleb>>ANGLETOFINESHIFT];

    //num = FixedMul(projection, sineb) << detailshift;
    //den = FixedMul(rw_distance, sinea);
    num = (projection * sineb);
    den = (rw_distance * sinea) >> FRACBITS;

    if (!den)
    {
        return 64 * FRACUNIT;
    }

//    uint16_t result = num / den;
//    return result;

    return (num / den);
}



#define PI 3.141592654f
//
// R_InitTables
//
void R_InitTables (void)
{
    // UNUSED: now getting from tables.c
#if 0
    int		i;
    float	a;
    float	fv;
    int		t;
    
    // viewangle tangent table
    for (i=0 ; i<FINEANGLES/2 ; i++)
    {
	a = (i-FINEANGLES/4+0.5)*PI*2/FINEANGLES;
	fv = FRACUNIT*tan (a);
	t = fv;
   // t = (int)(t / 256) * 256;
	finetangent[i] = t;
    }
    
    // finesine table
    for (i=0 ; i<5*FINEANGLES/4 ; i++)
    {
	// OPTIMIZE: mirror...
	a = (i+0.5)*PI*2/FINEANGLES;
	t = FRACUNIT*sin (a);
    //t = (int)(t / 256) * 256;
    finesine[i] = t;
    }
#endif

}



//
// R_InitTextureMapping
//
void R_InitTextureMapping (void)
{
    int			i;
    int			x;
    int			t;
    fixed_t		focallength;
    
    // Use tangent table to generate viewangletox:
    //  viewangletox will give the next greatest x
    //  after the view angle.
    //
    // Calc focallength
    //  so FIELDOFVIEW angles covers SCREENWIDTH.
    focallength = FixedDiv (centerxfrac,
			    finetangent[FINEANGLES/4+FIELDOFVIEW/2] );
	
    for (i=0 ; i<FINEANGLES/2 ; i++)
    {
	if (finetangent[i] > FRACUNIT*2)
	    t = -1;
	else if (finetangent[i] < -FRACUNIT*2)
	    t = viewwidth+1;
	else
	{
	    t = FixedMul (finetangent[i], focallength);
	    t = (centerxfrac - t+FRACUNIT-1)>>FRACBITS;

	    if (t < -1)
		t = -1;
	    else if (t>viewwidth+1)
		t = viewwidth+1;
	}
	viewangletox[i] = t;
    }
    
    // Scan viewangletox[] to generate xtoviewangle[]:
    //  xtoviewangle will give the smallest view angle
    //  that maps to x.	
    for (x=0;x<=viewwidth;x++)
    {
	i = 0;
	while (viewangletox[i]>x)
	    i++;
	xtoviewangle[x] = ((i<<ANGLETOFINESHIFT)-ANG90);
    }
    
    // Take out the fencepost cases from viewangletox.
    for (i=0 ; i<FINEANGLES/2 ; i++)
    {
	t = FixedMul (finetangent[i], focallength);
	t = centerx - t;
	
	if (viewangletox[i] == -1)
	    viewangletox[i] = 0;
	else if (viewangletox[i] == viewwidth+1)
	    viewangletox[i]  = viewwidth;
    }
	
    clipangle = xtoviewangle[0];
}



//
// R_InitLightTables
// Only inits the zlight table,
//  because the scalelight table changes with view size.
//
#define DISTMAP		2

//void R_InitLightTables (void)
//{
//    int		i;
//    int		j;
//    int		level;
//    int		startmap; 	
//    int		scale;
//    
//    // Calculate the light levels to use
//    //  for each level / distance combination.
//    for (i=0 ; i< LIGHTLEVELS ; i++)
//    {
//	startmap = ((LIGHTLEVELS-1-i)*2)*NUMCOLORMAPS/LIGHTLEVELS;
//	for (j=0 ; j<MAXLIGHTZ ; j++)
//	{
//	    scale = FixedDiv ((SCREENWIDTH/2*FRACUNIT), (j+1)<<LIGHTZSHIFT);
//	    scale >>= LIGHTSCALESHIFT;
//	    level = startmap - scale/DISTMAP;
//	    
//	    if (level < 0)
//		level = 0;
//
//	    if (level >= NUMCOLORMAPS)
//		level = NUMCOLORMAPS-1;
//
//	    zlight[i][j] = colormaps + level*256;
//	}
//    }
//}



//
// R_SetViewSize
// Do not really change anything here,
//  because it might be in the middle of a refresh.
// The change will take effect next refresh.
//
boolean		setsizeneeded;


void
R_SetViewSize (void)
{
    setsizeneeded = true;
}

void R_DrawColumnNoTexture(void)
{
    int count = dc_yh - dc_yl;
    if (count < 0)
        return;

    VLine(dc_x, dc_yl, count, 0x22);
}


//
// R_ExecuteSetViewSize
//
void R_ExecuteSetViewSize (void)
{
    fixed_t	cosadj;
    fixed_t	dy;
    int		i;
    int		j;
    int		level;
    int		startmap; 	

    setsizeneeded = false;

    scaledviewwidth = SCREENWIDTH;
    viewheight = SCREENHEIGHT;
    //if (setblocks == 11)
    //{
	//scaledviewwidth = SCREENWIDTH;
	//viewheight = SCREENHEIGHT;
    //}
    //else
    //{
	//scaledviewwidth = setblocks*32;
	//viewheight = (setblocks*168/10)&~7;
    //}
    
    //detailshift = setdetail;
    //viewwidth = scaledviewwidth>>detailshift;
    viewwidth = scaledviewwidth;
	
    centery = viewheight/2;
    centerx = viewwidth/2;
    centerxfrac = centerx<<FRACBITS;
    centeryfrac = centery<<FRACBITS;

#ifdef RENDER_DOUBLE_HEIGHT
    projection = (centerx * 6) / 5;
#else
    projection = (centerx * 12) / 5;
#endif

    //projection = centerx << 3;

    //if (!detailshift)
    //{
	colfunc = basecolfunc = R_DrawColumn;
	fuzzcolfunc = R_DrawFuzzColumn;
	transcolfunc = R_DrawTranslatedColumn;
	spanfunc = R_DrawSpan;
    //}
    //else
    //{
	//colfunc = basecolfunc = R_DrawColumnLow;
	//fuzzcolfunc = R_DrawFuzzColumnLow;
	//transcolfunc = R_DrawTranslatedColumnLow;
	//spanfunc = R_DrawSpanLow;
    //}

    R_InitBuffer (scaledviewwidth, viewheight);
	
    R_InitTextureMapping ();
    
    // psprite scales
    pspritescale = FRACUNIT*viewwidth/SCREENWIDTH;
    pspriteiscale = FRACUNIT*SCREENWIDTH/viewwidth;
    
    // thing clipping
    for (i=0 ; i<viewwidth ; i++)
	screenheightarray[i] = viewheight;
    
    // planes
    //for (i=0 ; i<viewheight ; i++)
    //{
	//dy = ((i-viewheight/2)<<FRACBITS)+FRACUNIT/2;
	//dy = abs(dy);
	//yslope[i] = FixedDiv ( (viewwidth<<detailshift)/2*FRACUNIT, dy);
    //}
	
    //for (i=0 ; i<viewwidth ; i++)
    //{
	//cosadj = abs(finecosine[xtoviewangle[i]>>ANGLETOFINESHIFT]);
	//distscale[i] = FixedDiv (FRACUNIT,cosadj);
    //}
    
    // Calculate the light levels to use
    //  for each level / scale combination.
    //for (i=0 ; i< LIGHTLEVELS ; i++)
    //{
	//startmap = ((LIGHTLEVELS-1-i)*2)*NUMCOLORMAPS/LIGHTLEVELS;
	//for (j=0 ; j<MAXLIGHTSCALE ; j++)
	//{
	//    level = startmap - j*SCREENWIDTH/(viewwidth<<detailshift)/DISTMAP;
	//    
	//    if (level < 0)
	//	level = 0;
    //
	//    if (level >= NUMCOLORMAPS)
	//	level = NUMCOLORMAPS-1;
    //
	//    scalelight[i][j] = colormaps + level*256;
	//}
    //}
}



//
// R_Init
//



void R_Init (void)
{
    //R_InitData ();
    R_InitPointToAngle ();
    R_InitTables ();
    // viewwidth / viewheight / detailLevel are set by the defaults

    R_SetViewSize ();
    //R_InitPlanes ();
    //R_InitLightTables ();
    R_InitSkyMap ();
    R_InitTranslationTables ();
	
    R_ExecuteSetViewSize();
    framecount = 0;
}


//
// R_PointInSubsector
//
const subsector_t*
R_PointInSubsector
( fixed_t	x,
  fixed_t	y )
{
    const node_t*	node;
    int		side;
    int		nodenum;

    // single subsector is a special case
    if (!map->numnodes)				
	return map->subsectors;
		
    nodenum = map->numnodes-1;

    while (! (nodenum & NF_SUBSECTOR) )
    {
	node = &map->nodes[nodenum];
	side = R_PointOnSide (x, y, node);
	nodenum = node->children[side];
    }
	
    return &map->subsectors[nodenum & ~NF_SUBSECTOR];
}



//
// R_SetupFrame
//
#if 0
void R_SetupFrame (player_t* player)
{		
    int		i;
    
    viewplayer = player;
    viewx = player->mo->x & 0xffff0000;
    viewy = player->mo->y & 0xffff0000;
    viewangle = (player->mo->angle + viewangleoffset) & 0xffff0000;
    extralight = player->extralight;

    viewz = player->viewz;
    
    viewsin = finesine[viewangle>>ANGLETOFINESHIFT];
    viewcos = finecosine[viewangle>>ANGLETOFINESHIFT];
	
    sscount = 0;
	
    if (player->fixedcolormap)
    {
	fixedcolormap =
	    colormaps
	    + player->fixedcolormap*256;
	
	walllights = scalelightfixed;

	for (i=0 ; i<MAXLIGHTSCALE ; i++)
	    scalelightfixed[i] = fixedcolormap;
    }
    else
	fixedcolormap = 0;
		
    framecount++;
    validcount++;
}
#else
void R_SetupFrame()
{
    viewangle &= 0xffff;
    viewsin = finesine[viewangle >> ANGLETOFINESHIFT];
    viewcos = finecosine[viewangle >> ANGLETOFINESHIFT];

    sscount = 0;

    framecount++;
    validcount++;
}
#endif



//
// R_RenderView
//
//void R_RenderPlayerView (player_t* player)
void R_RenderPlayerView(void)
{
    R_SetupFrame ();

    // Clear buffers.
    R_ClearClipSegs ();
    R_ClearDrawSegs ();
    R_ClearPlanes ();
    R_ClearSprites ();
    
    // check for new console commands.
    //NetUpdate ();

    // The head node is the last node output.
    R_RenderBSPNode (map->numnodes-1);
    
    // Check for new console commands.
    //NetUpdate ();
    
    //R_DrawPlanes ();
    
    // Check for new console commands.
    //NetUpdate ();
    
    R_DrawMasked ();

    // Check for new console commands.
    //NetUpdate ();				
}

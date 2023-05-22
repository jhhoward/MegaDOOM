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
//      Refresh/rendering module, shared data struct definitions.
//


#ifndef __R_DEFS__
#define __R_DEFS__


// Screenwidth.
#include "doomdef.h"

// Some more or less basic data types
// we depend on.
#include "m_fixed.h"

// We rely on the thinker data struct
// to handle sound origins in sectors.
//#include "d_think.h"
// SECTORS do store MObjs anyway.
//#include "p_mobj.h"

#include "i_video.h"

#include "v_patch.h"




// Silhouette, needed for clipping Segs (mainly)
// and sprites representing things.
#define SIL_NONE		0
#define SIL_BOTTOM		1
#define SIL_TOP			2
#define SIL_BOTH		3

#define MAXDRAWSEGS		256

#define NUM_LIGHTING_LEVELS 1
#define NUM_FLAT_LIGHTING_LEVELS 8


#if 0
//
// INTERNAL MAP TYPES
//  used by play and refresh
//

//
// Your plain vanilla vertex.
// Note: transformed values not buffered locally,
//  like some DOOM-alikes ("wt", "WebView") did.
//
typedef struct
{
    fixed_t	x;
    fixed_t	y;
    
} vertex_t;


// Forward of LineDefs, for Sectors.
struct line_s;

// Each sector has a degenmobj_t in its center
//  for sound origin purposes.
// I suppose this does not handle sound from
//  moving objects (doppler), because
//  position is prolly just buffered, not
//  updated.
typedef struct
{
    //thinker_t		thinker;	// not used for anything
    fixed_t		x;
    fixed_t		y;
    fixed_t		z;

} degenmobj_t;

//
// The SECTORS record, at runtime.
// Stores things/mobjs.
//
typedef	struct
{
    fixed_t	floorheight;
    fixed_t	ceilingheight;
    short	floorpic;
    short	ceilingpic;
    short	lightlevel;
    short	special;
    short	tag;

    // 0 = untraversed, 1,2 = sndlines -1
    int		soundtraversed;

    // thing that made a sound (or null)
    mobj_t*	soundtarget;

    // mapblock bounding box for height changes
    int		blockbox[4];

    // origin for any sounds played by the sector
    degenmobj_t	soundorg;

    // if == validcount, already checked
    int		validcount;

    // list of mobjs in sector
    mobj_t*	thinglist;

    // thinker_t for reversable actions
    void*	specialdata;

    int			linecount;
    struct line_s**	lines;	// [linecount] size
    
} sector_t;




//
// The SideDef.
//

typedef struct
{
    // add this to the calculated texture column
    fixed_t	textureoffset;
    
    // add this to the calculated texture top
    fixed_t	rowoffset;

    // Texture indices.
    // We do not maintain names here. 
    short	toptexture;
    short	bottomtexture;
    short	midtexture;

    // Sector the SideDef is facing.
    sector_t*	sector;
    
} side_t;



//
// Move clipping aid for LineDefs.
//
typedef enum
{
    ST_HORIZONTAL,
    ST_VERTICAL,
    ST_POSITIVE,
    ST_NEGATIVE

} slopetype_t;



typedef struct line_s
{
    // Vertices, from v1 to v2.
    vertex_t*	v1;
    vertex_t*	v2;

    // Precalculated v2 - v1 for side checking.
    fixed_t	dx;
    fixed_t	dy;

    // Animation related.
    short	flags;
    short	special;
    short	tag;

    // Visual appearance: SideDefs.
    //  sidenum[1] will be -1 if one sided
    short	sidenum[2];			

    // Neat. Another bounding box, for the extent
    //  of the LineDef.
    fixed_t	bbox[4];

    // To aid move clipping.
    slopetype_t	slopetype;

    // Front and back sector.
    // Note: redundant? Can be retrieved from SideDefs.
    sector_t*	frontsector;
    sector_t*	backsector;

    // if == validcount, already checked
    int		validcount;

    // thinker_t for reversable actions
    void*	specialdata;		
} line_t;




//
// A SubSector.
// References a Sector.
// Basically, this is a list of LineSegs,
//  indicating the visible walls that define
//  (all or some) sides of a convex BSP leaf.
//
typedef struct subsector_s
{
    sector_t*	sector;
    short	numlines;
    short	firstline;
    
} subsector_t;



//
// The LineSeg.
//
typedef struct
{
    vertex_t*	v1;
    vertex_t*	v2;
    
    fixed_t	offset;

    angle_t	angle;

    side_t*	sidedef;
    line_t*	linedef;

    // Sector references.
    // Could be retrieved from linedef, too.
    // backsector is NULL for one sided lines
    sector_t*	frontsector;
    sector_t*	backsector;
    
} seg_t;



//
// BSP node.
//
typedef struct
{
    // Partition line.
    fixed_t	x;
    fixed_t	y;
    fixed_t	dx;
    fixed_t	dy;

    // Bounding box for each child.
    fixed_t	bbox[2][4];

    // If NF_SUBSECTOR its a subsector.
    unsigned short children[2];
    
} node_t;




// PC direct to screen pointers
//B UNUSED - keep till detailshift in r_draw.c resolved
//extern byte*	destview;
//extern byte*	destscreen;





//
// OTHER TYPES
//

// This could be wider for >8 bit display.
// Indeed, true color support is posibble
//  precalculating 24bpp lightmap/colormap LUT.
//  from darkening PLAYPAL to all black.
// Could even us emore than 32 levels.
typedef pixel_t		lighttable_t;




//
// ?
//
typedef struct drawseg_s
{
    seg_t*		curline;
    int			x1;
    int			x2;

    fixed_t		scale1;
    fixed_t		scale2;
    fixed_t		scalestep;

    // 0=none, 1=bottom, 2=top, 3=both
    int			silhouette;

    // do not clip sprites above this
    fixed_t		bsilheight;

    // do not clip sprites below this
    fixed_t		tsilheight;
    
    // Pointers to lists for sprite clipping,
    //  all three adjusted so [x1] is first value.
    short*		sprtopclip;		
    short*		sprbottomclip;	
    short*		maskedtexturecol;
    
} drawseg_t;



// A vissprite_t is a thing
//  that will be drawn during a refresh.
// I.e. a sprite object that is partly visible.
typedef struct vissprite_s
{
    // Doubly linked list.
    struct vissprite_s*	prev;
    struct vissprite_s*	next;
    
    int			x1;
    int			x2;

    // for line side calculation
    fixed_t		gx;
    fixed_t		gy;		

    // global bottom / top for silhouette clipping
    fixed_t		gz;
    fixed_t		gzt;

    // horizontal position of x1
    fixed_t		startfrac;
    
    fixed_t		scale;
    
    // negative if flipped
    fixed_t		xiscale;	

    fixed_t		texturemid;
    int			patch;

    // for color translation and shadow draw,
    //  maxbright frames as well
    lighttable_t*	colormap;
   
    int			mobjflags;
    
} vissprite_t;


//	
// Sprites are patches with a special naming convention
//  so they can be recognized by R_InitSprites.
// The base name is NNNNFx or NNNNFxFx, with
//  x indicating the rotation, x = 0, 1-7.
// The sprite and frame specified by a thing_t
//  is range checked at run time.
// A sprite is a patch_t that is assumed to represent
//  a three dimensional object and may have multiple
//  rotations pre drawn.
// Horizontal flipping is used to save space,
//  thus NNNNF2F5 defines a mirrored patch.
// Some sprites will only have one picture used
// for all views: NNNNF0
//
typedef struct
{
    // If false use 0 for any position.
    // Note: as eight entries are available,
    //  we might as well insert the same name eight times.
    boolean	rotate;

    // Lump to use for view angles 0-7.
    short	lump[8];

    // Flip bit (1 = flip) to use for view angles 0-7.
    byte	flip[8];
    
} spriteframe_t;



//
// A sprite definition:
//  a number of animation frames.
//
typedef struct
{
    int			numframes;
    spriteframe_t*	spriteframes;

} spritedef_t;



//
// Now what is a visplane, anyway?
// 
typedef struct
{
  fixed_t		height;
  int			picnum;
  int			lightlevel;
  int			minx;
  int			maxx;
  
  // leave pads for [minx-1]/[maxx+1]
  
  byte		pad1;
  // Here lies the rub for all
  //  dynamic resize/change of resolution.
  byte		top[SCREENWIDTH];
  byte		pad2;
  byte		pad3;
  // See above.
  byte		bottom[SCREENWIDTH];
  byte		pad4;

} visplane_t;

#else

typedef struct
{
    int16_t x;
    int16_t y;
} vertex_t;

struct line_s;

typedef	struct
{
    int16_t	floorheight;
    int16_t	ceilingheight;
    int16_t	floorpic;
    int16_t	ceilingpic;
    int16_t	lightlevel;
    int16_t	special;
    int16_t	tag;

    //    int			linecount;
    //    const struct line_s** lines;	// [linecount] size
} sector_t;

//
// The SideDef.
//

typedef struct
{
    // add this to the calculated texture column
    int16_t	textureoffset;

    // add this to the calculated texture top
    int16_t	rowoffset;

    // Texture indices.
    // We do not maintain names here. 
    int16_t	toptexture;
    int16_t	bottomtexture;
    int16_t	midtexture;

    // Sector the SideDef is facing.
    const sector_t* sector;
} side_t;

typedef struct line_s
{
    // Vertices, from v1 to v2.
    const vertex_t* v1;
    const vertex_t* v2;

    // Precalculated v2 - v1 for side checking.
    int16_t	dx;
    int16_t	dy;

    // Animation related.
    short	flags;
    short	special;
    short	tag;

    // Visual appearance: SideDefs.
    //  sidenum[1] will be -1 if one sided
    short	sidenum[2];

    // Neat. Another bounding box, for the extent
    //  of the LineDef.
    //int16_t	bbox[4];

    // To aid move clipping.
    //slopetype_t	slopetype;

    // Front and back sector.
    // Note: redundant? Can be retrieved from SideDefs.
    const sector_t* frontsector;
    const sector_t* backsector;

    // if == validcount, already checked
    //int		validcount;

    // thinker_t for reversable actions
    //void* specialdata;
} line_t;

//
// The LineSeg.
//
typedef struct
{
    const vertex_t* v1;
    const vertex_t* v2;

    int16_t	offset;

    uint16_t	angle;

    const side_t* sidedef;
    const line_t* linedef;

    //int16_t length;

    // Sector references.
    // Could be retrieved from linedef, too.
    // backsector is NULL for one sided lines
    const sector_t* frontsector;
    const sector_t* backsector;

} seg_t;

//
// BSP node.
//
typedef struct
{
    // Partition line.
    int16_t	x;
    int16_t	y;
    int16_t	dx;
    int16_t	dy;

    // Bounding box for each child.
    int16_t	bbox[2][4];

    // If NF_SUBSECTOR its a subsector.
    uint16_t children[2];

} node_t;

//
// A SubSector.
// References a Sector.
// Basically, this is a list of LineSegs,
//  indicating the visible walls that define
//  (all or some) sides of a convex BSP leaf.
//
typedef struct subsector_s
{
    int16_t	numlines;
    int16_t	firstline;
    const sector_t* sector;
} subsector_t;

typedef struct
{
    const vertex_t* vertices;
    const side_t* sides;
    const node_t* nodes;
    const subsector_t* subsectors;
    const seg_t* segs;
    const line_t* lines;
    const sector_t* sectors;
    const struct mapthing_s* things;

    int numnodes;
} map_t;

typedef struct
{
    int width, height;
    uint8_t colour[NUM_LIGHTING_LEVELS];
    const uint32_t columns[NUM_LIGHTING_LEVELS];
} walltexture_t;

extern const walltexture_t walltextures[];
extern const uint32_t texturecolumns[];
extern const uint8_t textureatlas[];

typedef struct
{
    uint8_t colour[NUM_FLAT_LIGHTING_LEVELS];
} flat_t;


// A vissprite_t is a thing
//  that will be drawn during a refresh.
// I.e. a sprite object that is partly visible.
typedef struct vissprite_s
{
    // Doubly linked list.
    struct vissprite_s* prev;
    struct vissprite_s* next;

    int			x1;
    int			x2;

    // for line side calculation
    fixed_t		gx;
    fixed_t		gy;

    // global bottom / top for silhouette clipping
    fixed_t		gz;
    fixed_t		gzt;

    // horizontal position of x1
    fixed_t		startfrac;

    fixed_t		scale;

    // negative if flipped
    fixed_t		xiscale;

    fixed_t		texturemid;
    int			patch;

    // for color translation and shadow draw,
    //  maxbright frames as well
    //lighttable_t* colormap;

    int			mobjflags;

} vissprite_t;


//	
// Sprites are patches with a special naming convention
//  so they can be recognized by R_InitSprites.
// The base name is NNNNFx or NNNNFxFx, with
//  x indicating the rotation, x = 0, 1-7.
// The sprite and frame specified by a thing_t
//  is range checked at run time.
// A sprite is a patch_t that is assumed to represent
//  a three dimensional object and may have multiple
//  rotations pre drawn.
// Horizontal flipping is used to save space,
//  thus NNNNF2F5 defines a mirrored patch.
// Some sprites will only have one picture used
// for all views: NNNNF0
//
typedef struct
{
    // If false use 0 for any position.
    // Note: as eight entries are available,
    //  we might as well insert the same name eight times.
    boolean	rotate;

    // Lump to use for view angles 0-7.
    short	lump[8];

    // Flip bit (1 = flip) to use for view angles 0-7.
    byte	flip[8];

} spriteframe_t;



//
// A sprite definition:
//  a number of animation frames.
//
typedef struct
{
    int			numframes;
    spriteframe_t* spriteframes;

} spritedef_t;

//
// ?
//
typedef struct drawseg_s
{
    const seg_t* curline;
    int			x1;
    int			x2;

    fixed_t		scale1;
    fixed_t		scale2;
    fixed_t		scalestep;

    // 0=none, 1=bottom, 2=top, 3=both
    int			silhouette;

    // do not clip sprites above this
    fixed_t		bsilheight;

    // do not clip sprites below this
    fixed_t		tsilheight;

    // Pointers to lists for sprite clipping,
    //  all three adjusted so [x1] is first value.
    short* sprtopclip;
    short* sprbottomclip;
    short* maskedtexturecol;

} drawseg_t;

#endif


#endif

#pragma once

#include "doomtypes.h"

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

    int			linecount;
    struct line_s** lines;	// [linecount] size
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
    sector_t* sector;
} side_t;

typedef struct line_s
{
    // Vertices, from v1 to v2.
    vertex_t* v1;
    vertex_t* v2;

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
    sector_t* frontsector;
    sector_t* backsector;

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
    vertex_t* v1;
    vertex_t* v2;

    int16_t	offset;

    int16_t	angle;

    side_t* sidedef;
    line_t* linedef;

    // Sector references.
    // Could be retrieved from linedef, too.
    // backsector is NULL for one sided lines
    //sector_t* frontsector;
    //sector_t* backsector;

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
   // sector_t* sector;
    int16_t	numlines;
    int16_t	firstline;

} subsector_t;

typedef struct
{
    vertex_t* vertices;
    side_t* sides;
    node_t* nodes;
    subsector_t* subsectors;
    seg_t* segs;
    line_t* lines;
    sector_t* sectors;
    struct mapthing_s* things;

    uint16_t rootnode;
} map_t;


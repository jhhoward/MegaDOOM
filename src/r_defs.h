#pragma once

#include "doomtypes.h"

#define VIEWPORT_WIDTH 112
#define VIEWPORT_HALF_WIDTH (VIEWPORT_WIDTH / 2)
#define VIEWPORT_HEIGHT 128
#define VIEWPORT_HALF_HEIGHT (VIEWPORT_HEIGHT / 2)

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

    int16_t length;

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
    const vertex_t* vertices;
    const side_t* sides;
    const node_t* nodes;
    const subsector_t* subsectors;
    const seg_t* segs;
    const line_t* lines;
    const sector_t* sectors;
    const struct mapthing_s* things;

    uint16_t rootnode;
} map_t;

typedef struct
{
    int width, height;
    const uint8_t* const* columns;
} walltexture_t;

typedef struct
{
    uint8_t colour[8];
} flat_t;

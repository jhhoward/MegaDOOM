#ifndef _MAP_H_
#define _MAP_H_

#include <stdint.h>
#include "DoomData.h"

typedef struct
{
	int numVertices;
	mapvertex_t* vertices;

	int numSides;
	mapsidedef_t* sides;

	int numNodes;
	mapnode_t* nodes;

	int numSubsectors;
	mapsubsector_t* subsectors;

	int numSegs;
	mapseg_t* segs;

	int numLines;
	maplinedef_t* lines;

	int numSectors;
	mapsector_t* sectors;
} map_t;

#ifdef __cplusplus
extern "C" {
#endif

	extern map_t Map;

#ifdef __cplusplus
};
#endif

#endif



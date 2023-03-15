#include <stdio.h>
#include "doomtypes.h"
#include <memory.h>
#include <malloc.h>
#include <string.h>
#include "Map.h"
#include "DoomData.h"
#include "r_defs.h"


#pragma warning(disable:4996)

typedef struct
{
	char sig[4];
	int32_t numFiles;
	int32_t	offFAT;
} wad_file_header_t;

typedef struct
{
	int32_t offData;
	int32_t lenData;
	char name[8];
} wad_file_entry_t;

void LoadMapFromWad(mapdata_t* map, const char* filename, const char* levelLabel)
{
	FILE* fs = fopen(filename, "rb");
	wad_file_header_t header;

	if (!fs)
	{
		return;
	}

	fread(&header, sizeof(wad_file_header_t), 1, fs);

	printf("WAD type: %c%c%c%c\n", header.sig[0], header.sig[1], header.sig[2], header.sig[3]);
	printf("Num files: %d\n", header.numFiles);

	fseek(fs, header.offFAT, SEEK_SET);

	wad_file_entry_t* files = malloc(sizeof(wad_file_entry_t) * header.numFiles);
	int levelIndex = -1;

	for (int n = 0; n < header.numFiles; n++)
	{
		char fileName[9];
		fileName[8] = '\0';
		wad_file_entry_t* fileEntry = &files[n];
		fread(fileEntry, sizeof(wad_file_entry_t), 1, fs);
		memcpy(fileName, fileEntry->name, 8);
		printf("File: %s length: %d offset: %d\n", fileName, fileEntry->lenData, fileEntry->offData);

		if (!strncmp(fileName, levelLabel, 4))
		{
			levelIndex = n;
		}
	}

	wad_file_entry_t* file;
	
	file = &files[ML_NODES + levelIndex];
	map->numNodes = file->lenData / sizeof(mapnode_t);
	map->nodes = (mapnode_t*) malloc(file->lenData);
	fseek(fs, file->offData, SEEK_SET);
	fread(map->nodes, 1, file->lenData, fs);

	file = &files[ML_SSECTORS + levelIndex];
	map->numSubsectors = file->lenData / sizeof(mapsubsector_t);
	map->subsectors = (mapsubsector_t*)malloc(file->lenData);
	fseek(fs, file->offData, SEEK_SET);
	fread(map->subsectors, 1, file->lenData, fs);

	file = &files[ML_SEGS + levelIndex];
	map->numSegs = file->lenData / sizeof(mapseg_t);
	map->segs = (mapseg_t*)malloc(file->lenData);
	fseek(fs, file->offData, SEEK_SET);
	fread(map->segs, 1, file->lenData, fs);

	file = &files[ML_VERTEXES + levelIndex];
	map->numVertices = file->lenData / sizeof(mapvertex_t);
	map->vertices = (mapvertex_t*)malloc(file->lenData);
	fseek(fs, file->offData, SEEK_SET);
	fread(map->vertices, 1, file->lenData, fs);

	file = &files[ML_LINEDEFS + levelIndex];
	map->numLines = file->lenData / sizeof(maplinedef_t);
	map->lines = (maplinedef_t*)malloc(file->lenData);
	fseek(fs, file->offData, SEEK_SET);
	fread(map->lines, 1, file->lenData, fs);

	file = &files[ML_SIDEDEFS + levelIndex];
	map->numSides = file->lenData / sizeof(mapsidedef_t);
	map->sides = (mapsidedef_t*)malloc(file->lenData);
	fseek(fs, file->offData, SEEK_SET);
	fread(map->sides, 1, file->lenData, fs);

	file = &files[ML_SECTORS + levelIndex];
	map->numSectors = file->lenData / sizeof(mapsector_t);
	map->sectors = (mapsector_t*)malloc(file->lenData);
	fseek(fs, file->offData, SEEK_SET);
	fread(map->sectors, 1, file->lenData, fs);

	file = &files[ML_THINGS + levelIndex];
	map->numThings = file->lenData / sizeof(mapthing_t);
	map->things = (mapthing_t*)malloc(file->lenData);
	fseek(fs, file->offData, SEEK_SET);
	fread(map->things, 1, file->lenData, fs);

	fclose(fs);
}

void ExtractMapData(mapdata_t* src, map_t* dest)
{
	dest->lines = malloc(src->numLines * sizeof(line_t));
	dest->nodes = malloc(src->numNodes * sizeof(node_t));
	dest->sectors = malloc(src->numSectors * sizeof(sector_t));
	dest->segs = malloc(src->numSegs * sizeof(seg_t));
	dest->sides = malloc(src->numSides * sizeof(side_t));
	dest->subsectors = malloc(src->numSubsectors * sizeof(subsector_t));
	dest->vertices = malloc(src->numVertices * sizeof(vertex_t));
	dest->things = malloc(src->numThings * sizeof(mapthing_t));

	// Can just memcpy vertices
	memcpy((void*) dest->vertices, src->vertices, src->numVertices * sizeof(vertex_t));

	// Sides
	for (int n = 0; n < src->numSides; n++)
	{
		mapsidedef_t* srcside = &src->sides[n];
		side_t* destside = (side_t*)&dest->sides[n];
		destside->textureoffset = srcside->textureoffset;
		destside->toptexture = srcside->toptexture[0] == '-' ? 0 : 1;
		destside->midtexture = srcside->midtexture[0] == '-' ? 0 : 1;
		destside->bottomtexture = srcside->bottomtexture[0] == '-' ? 0 : 1;
		destside->rowoffset = srcside->rowoffset;
		destside->sector = &dest->sectors[srcside->sector];
	}

	// Lines
	for (int n = 0; n < src->numLines; n++)
	{
		maplinedef_t* srcline = &src->lines[n];
		line_t* destline = (line_t*)&dest->lines[n];
		destline->v1 = &dest->vertices[srcline->v1];
		destline->v2 = &dest->vertices[srcline->v2];
		destline->dx = destline->v2->x - destline->v1->x;
		destline->dy = destline->v2->y - destline->v1->y;
		destline->flags = srcline->flags;
		destline->special = srcline->special;
		destline->tag = srcline->tag;
		destline->sidenum[0] = srcline->sidenum[0];
		destline->sidenum[1] = srcline->sidenum[1];

		if (srcline->sidenum[0] != -1)
			destline->frontsector = dest->sides[srcline->sidenum[0]].sector;
		else
			destline->frontsector = NULL;

		if (srcline->sidenum[1] != -1)
			destline->backsector = dest->sides[srcline->sidenum[1]].sector;
		else
			destline->backsector = NULL;
	}

	// Segs
	for (int n = 0; n < src->numSegs; n++)
	{
		mapseg_t* srcseg = &src->segs[n];
		seg_t* destseg = (seg_t*) &dest->segs[n];
		destseg->v1 = &dest->vertices[srcseg->v1];
		destseg->v2 = &dest->vertices[srcseg->v2];
		destseg->offset = srcseg->offset;
		destseg->angle = srcseg->angle;
		destseg->linedef = &dest->lines[srcseg->linedef];
		destseg->sidedef = &dest->sides[destseg->linedef->sidenum[srcseg->side]];
	}

	// Sectors
	for (int n = 0; n < src->numSectors; n++)
	{
		mapsector_t* srcsector = &src->sectors[n];
		sector_t* destsector = (sector_t*) &dest->sectors[n];

		destsector->floorheight = srcsector->floorheight;
		destsector->ceilingheight = srcsector->ceilingheight;
		destsector->lightlevel = srcsector->lightlevel;
		destsector->special = srcsector->special;
		destsector->tag = srcsector->tag;
		destsector->floorpic = 0;
		destsector->ceilingpic = 0;
	}

	// Nodes
	for (int n = 0; n < src->numNodes; n++)
	{
		mapnode_t* srcnode = &src->nodes[n];
		node_t* destnode = (node_t*) &dest->nodes[n];
		// Structs are identical, just memcpy
		memcpy(destnode, srcnode, sizeof(node_t));
	}

	// Subsectors
	for (int n = 0; n < src->numSubsectors; n++)
	{
		mapsubsector_t* srcsubsector = &src->subsectors[n];
		subsector_t* destsubsector = (subsector_t*) &dest->subsectors[n];

		destsubsector->numlines = srcsubsector->numsegs;
		destsubsector->firstline = srcsubsector->firstseg;
	}

	// Things
	memcpy((void*) dest->things, src->things, src->numThings * sizeof(mapthing_t));

	dest->rootnode = src->numNodes - 1;
}
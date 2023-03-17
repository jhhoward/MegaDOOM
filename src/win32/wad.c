#include <stdio.h>
#include "doomtypes.h"
#include <memory.h>
#include <malloc.h>
#include <string.h>
#include "Map.h"
#include "DoomData.h"
#include "r_defs.h"
#include "wad.h"

#pragma warning(disable:4996)

wad_file_t* LoadWad(const char* filename)
{
	FILE* fs = fopen(filename, "rb");

	if (!fs)
	{
		return NULL;
	}

	fseek(fs, 0, SEEK_END);

	wad_file_t* wad = (wad_file_t*)malloc(sizeof(wad_file_t));

	long datalen = ftell(fs);
	wad->data = (uint8_t*) malloc(datalen);
	fseek(fs, 0, SEEK_SET);
	fread(wad->data, 1, datalen, fs);
	fclose(fs);

	wad->header = (wad_file_header_t*)(wad->data);
	wad->files = (wad_file_entry_t*)(wad->data + wad->header->offFAT);

	for (int n = 0; n < wad->header->numFiles; n++)
	{
		wad_file_entry_t* fileEntry = &wad->files[n];
	
		char fileName[9];
		fileName[8] = '\0';
	
		memcpy(fileName, fileEntry->name, 8);
		printf("File: %s length: %d offset: %d\n", fileName, fileEntry->lenData, fileEntry->offData);
	}
	return wad;
}

int FindWadEntry(wad_file_t* wad, const char* entryName, int start)
{
	uint32_t* name = (uint32_t*)(entryName);
	
	for (int n = start; n < wad->header->numFiles; n++)
	{
		uint32_t* entryName = (uint32_t*) wad->files[n].name;
		if (name[0] == entryName[0] && name[1] == entryName[1])
		{
			return n;
		}
	}

	return -1;
}

void* GetWadFileData(wad_file_t* wad, int index)
{
	return wad->data + wad->files[index].offData;
}

int32_t GetWadFileLength(wad_file_t* wad, int index)
{
	return wad->files[index].lenData;
}

bool LoadMapDataFromWad(wad_file_t* wad, const char* levelName, mapdata_t* map)
{
	int index = FindWadEntry(wad, levelName, 0);

	if (index != -1)
	{
		map->nodes = (mapnode_t*) GetWadFileData(wad, ML_NODES + index);
		map->numNodes = GetWadFileLength(wad, ML_NODES + index) / sizeof(mapnode_t);
		map->subsectors = (mapsubsector_t*)GetWadFileData(wad, ML_SSECTORS + index);
		map->numSubsectors = GetWadFileLength(wad, ML_SSECTORS + index) / sizeof(mapsubsector_t);
		map->segs = (mapseg_t*)GetWadFileData(wad, ML_SEGS + index);
		map->numSegs = GetWadFileLength(wad, ML_SEGS + index) / sizeof(mapseg_t);
		map->vertices = (mapvertex_t*)GetWadFileData(wad, ML_VERTEXES + index);
		map->numVertices = GetWadFileLength(wad, ML_VERTEXES + index) / sizeof(mapvertex_t);
		map->lines = (maplinedef_t*)GetWadFileData(wad, ML_LINEDEFS + index);
		map->numLines = GetWadFileLength(wad, ML_LINEDEFS + index) / sizeof(maplinedef_t);
		map->sides = (mapsidedef_t*)GetWadFileData(wad, ML_SIDEDEFS + index);
		map->numSides = GetWadFileLength(wad, ML_SIDEDEFS + index) / sizeof(mapsidedef_t);
		map->sectors = (mapsector_t*)GetWadFileData(wad, ML_SECTORS + index);
		map->numSectors = GetWadFileLength(wad, ML_SECTORS + index) / sizeof(mapsector_t);
		map->things = (mapthing_t*)GetWadFileData(wad, ML_THINGS + index);
		map->numThings = GetWadFileLength(wad, ML_THINGS + index) / sizeof(mapthing_t);
		return true;
	}
	return false;
}

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
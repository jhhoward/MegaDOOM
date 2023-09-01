#pragma once

#include <stdbool.h>
#include <stdint.h>
#include "doomtype.h"
#include "r_defs.h"
#include "DoomData.h"

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

typedef struct
{
	wad_file_header_t* header;
	wad_file_entry_t* files;
	uint8_t* data;
} wad_file_t;

void LoadMapFromWad(mapdata_t* map, const char* filename, const char* levelLabel);

void ExtractMapData(mapdata_t* src, map_t* dest);
wad_file_t* LoadWad(const char* filename);
int FindWadEntry(wad_file_t* wad, const char* entryName, int start);
void* GetWadFileData(wad_file_t* wad, int index);
bool LoadMapDataFromWad(wad_file_t* wad, const char* levelName, mapdata_t* map);

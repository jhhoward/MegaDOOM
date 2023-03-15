#include <stdio.h>
#include <stdlib.h>
#include "../src/DoomData.h"

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

void LoadWad(const char* filename)
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
	const char* levelLabel = "E1M1";

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
	Map.numNodes = file->lenData / sizeof(mapnode_t);
	Map.nodes = (mapnode_t*) malloc(file->lenData);
	fseek(fs, file->offData, SEEK_SET);
	fread(Map.nodes, 1, file->lenData, fs);

	file = &files[ML_SSECTORS + levelIndex];
	Map.numSubsectors = file->lenData / sizeof(mapsubsector_t);
	Map.subsectors = (mapsubsector_t*)malloc(file->lenData);
	fseek(fs, file->offData, SEEK_SET);
	fread(Map.subsectors, 1, file->lenData, fs);

	file = &files[ML_SEGS + levelIndex];
	Map.numSegs = file->lenData / sizeof(mapseg_t);
	Map.segs = (mapseg_t*)malloc(file->lenData);
	fseek(fs, file->offData, SEEK_SET);
	fread(Map.segs, 1, file->lenData, fs);

	file = &files[ML_VERTEXES + levelIndex];
	Map.numVertices = file->lenData / sizeof(mapvertex_t);
	Map.vertices = (mapvertex_t*)malloc(file->lenData);
	fseek(fs, file->offData, SEEK_SET);
	fread(Map.vertices, 1, file->lenData, fs);

	file = &files[ML_LINEDEFS + levelIndex];
	Map.numLines = file->lenData / sizeof(maplinedef_t);
	Map.lines = (maplinedef_t*)malloc(file->lenData);
	fseek(fs, file->offData, SEEK_SET);
	fread(Map.lines, 1, file->lenData, fs);

	file = &files[ML_SIDEDEFS + levelIndex];
	Map.numSides = file->lenData / sizeof(mapsidedef_t);
	Map.sides = (mapsidedef_t*)malloc(file->lenData);
	fseek(fs, file->offData, SEEK_SET);
	fread(Map.sides, 1, file->lenData, fs);

	file = &files[ML_SECTORS + levelIndex];
	Map.numSectors = file->lenData / sizeof(mapsector_t);
	Map.sectors = (mapsector_t*)malloc(file->lenData);
	fseek(fs, file->offData, SEEK_SET);
	fread(Map.sectors, 1, file->lenData, fs);


	fclose(fs);
}

int main(int argc, char* argv[])
{
	return 0;
}

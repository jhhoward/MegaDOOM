#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include "r_defs.h"
#include "DoomData.h"
#include "win32/wad.h"
#include "win32/lodepng.h"

#pragma warning(disable:4996)

uint32_t doompalette[256];

int16_t LookupTexture(char name[8])
{
	return name[0] == '-' ? 0 : 1;
}

void DumpMapToHeader(mapdata_t* mapdata, const char* levelname)
{
	char filename[50];
	sprintf_s(filename, 50, "%s.inc.h", levelname);

	FILE* fs = fopen(filename, "w");

	fprintf(fs, "// Level data for %s\n", levelname);
	
	// Vertices
	fprintf(fs, "const vertex_t %s_vertices[] = {\n", levelname);
	for (int n = 0; n < mapdata->numVertices; n++)
	{
		fprintf(fs, "\t{%d,%d},\n", mapdata->vertices[n].x, mapdata->vertices[n].y);
	}
	fprintf(fs, "};\n");

	// Sectors
	fprintf(fs, "const sector_t %s_sectors[] = {\n", levelname);
	for (int n = 0; n < mapdata->numSectors; n++)
	{
		mapsector_t* sector = &mapdata->sectors[n];
		fprintf(fs, "\t{ %d, %d, %d, %d, %d, %d, %d },\n",
			sector->floorheight,
			sector->ceilingheight,
			LookupTexture(sector->floorpic),
			LookupTexture(sector->ceilingpic),
			sector->lightlevel,
			sector->special,
			sector->tag
			);
	}
	fprintf(fs, "};\n");

	// Subsectors
	fprintf(fs, "const subsector_t %s_subsectors[] = {\n", levelname);
	for (int n = 0; n < mapdata->numSubsectors; n++)
	{
		mapsubsector_t* subsector = &mapdata->subsectors[n];

		fprintf(fs, "\t{ %d, %d },\n",
			subsector->numsegs,
			subsector->firstseg);
	}
	fprintf(fs, "};\n");


	// Nodes
	fprintf(fs, "const node_t %s_nodes[] = {\n", levelname);
	for (int n = 0; n < mapdata->numNodes; n++)
	{
		mapnode_t* node = &mapdata->nodes[n];
		fprintf(fs, "\t{ ");
		fprintf(fs, "%d, ", node->x);
		fprintf(fs, "%d, ", node->y);
		fprintf(fs, "%d, ", node->dx);
		fprintf(fs, "%d, ", node->dy);

		fprintf(fs, "{ ");

		for (int i = 0; i < 2; i++)
		{
			fprintf(fs, "{ ");

			for (int j = 0; j < 4; j++)
			{
				fprintf(fs, "%d, ", node->bbox[i][j]);
			}

			fprintf(fs, "}, ");
		}

		fprintf(fs, "}, ");

		fprintf(fs, "{%d, %d}", node->children[0], node->children[1]);

		fprintf(fs, "},\n");

	}
	fprintf(fs, "};\n");

	// Lines
	fprintf(fs, "const line_t %s_lines[] = {\n", levelname);
	for (int n = 0; n < mapdata->numLines; n++)
	{
		maplinedef_t* line = &mapdata->lines[n];

		fprintf(fs, "\t{ ");
		fprintf(fs, "&%s_vertices[%d], ", levelname, line->v1);
		fprintf(fs, "&%s_vertices[%d], ", levelname, line->v2);

		fprintf(fs, "%d, ", mapdata->vertices[line->v2].x - mapdata->vertices[line->v1].x);
		fprintf(fs, "%d, ", mapdata->vertices[line->v2].y - mapdata->vertices[line->v1].y);

		fprintf(fs, "%d, ", line->flags);
		fprintf(fs, "%d, ", line->special);
		fprintf(fs, "%d, ", line->tag);

		fprintf(fs, "{%d, %d}, ", line->sidenum[0], line->sidenum[1]);

		if (line->sidenum[0] != -1)
			fprintf(fs, "&%s_sectors[%d], ", levelname, mapdata->sides[line->sidenum[0]].sector);
		else
			fprintf(fs, "0, ");

		if (line->sidenum[1] != -1)
			fprintf(fs, "&%s_sectors[%d]", levelname, mapdata->sides[line->sidenum[1]].sector);
		else
			fprintf(fs, "0");

		fprintf(fs, "},\n");
	}
	fprintf(fs, "};\n");

	// Sides
	fprintf(fs, "const side_t %s_sides[] = {\n", levelname);
	for (int n = 0; n < mapdata->numSides; n++)
	{
		mapsidedef_t* side = &mapdata->sides[n];

		fprintf(fs, "\t{ ");
		fprintf(fs, "%d, ", side->textureoffset);
		fprintf(fs, "%d, ", side->rowoffset);
		fprintf(fs, "%d, ", LookupTexture(side->toptexture));
		fprintf(fs, "%d, ", LookupTexture(side->bottomtexture));
		fprintf(fs, "%d, ", LookupTexture(side->midtexture));
		fprintf(fs, "&%s_sectors[%d], ", levelname, side->sector);
		fprintf(fs, "},\n");
	}
	fprintf(fs, "};\n");

	// Segs
	fprintf(fs, "const seg_t %s_segs[] = {\n", levelname);
	for (int n = 0; n < mapdata->numSegs; n++)
	{
		mapseg_t* seg = &mapdata->segs[n];

		fprintf(fs, "\t{ ");
		fprintf(fs, "&%s_vertices[%d], ", levelname, seg->v1);
		fprintf(fs, "&%s_vertices[%d], ", levelname, seg->v2);
		fprintf(fs, "%d, ", seg->offset);
		fprintf(fs, "%d, ", seg->angle);
		fprintf(fs, "&%s_sides[%d], ", levelname, mapdata->lines[seg->linedef].sidenum[seg->side]);
		fprintf(fs, "&%s_lines[%d]", levelname, seg->linedef);
		fprintf(fs, "},\n");
	}
	fprintf(fs, "\n};\n");

	// Things
	fprintf(fs, "const mapthing_t %s_things[] = {\n", levelname);
	for (int n = 0; n < mapdata->numThings; n++)
	{
		mapthing_t* thing = &mapdata->things[n];

		fprintf(fs, "\t{ ");
		fprintf(fs, "%d, ", thing->x);
		fprintf(fs, "%d, ", thing->y);
		fprintf(fs, "%d, ", thing->angle);
		fprintf(fs, "%d, ", thing->type);
		fprintf(fs, "%d", thing->options);
		fprintf(fs, "},\n");
	}
	fprintf(fs, "\n};\n");

	fprintf(fs, "const map_t map_%s = {\n", levelname);
	fprintf(fs, "\t%s_vertices,\n", levelname);
	fprintf(fs, "\t%s_sides,\n", levelname);
	fprintf(fs, "\t%s_nodes,\n", levelname);
	fprintf(fs, "\t%s_subsectors,\n", levelname);
	fprintf(fs, "\t%s_segs,\n", levelname);
	fprintf(fs, "\t%s_lines,\n", levelname);
	fprintf(fs, "\t%s_sectors,\n", levelname);
	fprintf(fs, "\t%s_things,\n", levelname);
	fprintf(fs, "\t%d,\n", mapdata->numNodes - 1);
	fprintf(fs, "};\n\n");

	//side_t* sides;
	//node_t* nodes;
	//subsector_t* subsectors;
	//seg_t* segs;
	//line_t* lines;
	//sector_t* sectors;
	//uint16_t rootnode;

	fclose(fs);
}

typedef struct
{
	uint16_t width;
	uint16_t height;
	int16_t leftoffset;
	int16_t topoffset;
} patch_header_t;

void ExtractPatch(wad_file_t* wad, int index)
{
	wad_file_entry_t* file = &wad->files[index];

	uint8_t* data = wad->data + file->offData;
	patch_header_t* header = (patch_header_t*)(data);
	uint32_t* columnofs = (uint32_t*)(data + sizeof(patch_header_t));

	uint32_t* pixels = malloc(header->width * header->height * sizeof(uint32_t));
	memset(pixels, 0, header->width * header->height * sizeof(uint32_t));

	for (int x = 0; x < header->width; x++)
	{
		uint8_t* ptr = data + columnofs[x];
		uint8_t topdelta;
		int y = 0;
		
		topdelta = *ptr++;
		while (topdelta != 0xff)
		{
			y = topdelta;

			uint8_t length = *ptr++;
			ptr++; // padding

			while (length--)
			{
				uint8_t pixel = *ptr++;
				pixels[y * header->width + x] = doompalette[pixel];
				y++;
			}

			ptr++; // padding
			topdelta = *ptr++;
		}
	}

	char filename[10];
	sprintf(filename, "%d.png", index);
	lodepng_encode32_file(filename, (uint8_t*) pixels, header->width, header->height);
}

void ExtractPatches(wad_file_t* wad)
{
	int start = FindWadEntry(wad, "P_START", 0);
	int end = FindWadEntry(wad, "P_END", 0);

	if (start >= 0 && end > start)
	{
		for (int n = start + 1; n < end; n++)
		{
			char name[9];
			name[8] = 0;
			memcpy(name, wad->files[n].name, 8);
			printf("%d : %s\n", n, name);

			if (wad->files[n].lenData > 0)
			{
				ExtractPatch(wad, n);
			}
		}
	}

	for (int n = 0; n < wad->header->numFiles; n++)
	{
		if (wad->files[n].name[0] == 'S' && wad->files[n].name[1] == 'T')
		{
			ExtractPatch(wad, n);
		}
	}
}

void ExtractFlat(wad_file_t* wad, int index)
{
	uint8_t* data = (uint8_t*) GetWadFileData(wad, index);
	uint32_t pixels[64 * 64];

	for (int n = 0; n < 64 * 64; n++)
	{
		pixels[n] = doompalette[data[n]];
	}

	char filename[32];
	sprintf(filename, "%d.png", index);
	lodepng_encode32_file(filename, (uint8_t*)pixels, 64, 64);

	int r = 0, g = 0, b = 0;
	for (int n = 0; n < 64 * 64; n++)
	{
		r += pixels[n] & 0xff;
		g += (pixels[n] >> 8) & 0xff;
		b += (pixels[n] >> 16) & 0xff;
	}

	r /= (64 * 64);
	g /= (64 * 64);
	b /= (64 * 64);
	uint32_t average = 0xff000000 | (r) | (g << 8) | (b << 16);

	for (int n = 0; n < 64 * 64; n++)
	{
		pixels[n] = average;
	}

	sprintf(filename, "%d_avg.png", index);
	lodepng_encode32_file(filename, (uint8_t*)pixels, 64, 64);
}

void ExtractFlats(wad_file_t* wad)
{
	int start = FindWadEntry(wad, "F_START", 0);
	int end = FindWadEntry(wad, "F_END", 0);

	if (start >= 0 && end > start)
	{
		for (int n = start + 1; n < end; n++)
		{
			char name[9];
			name[8] = 0;
			memcpy(name, wad->files[n].name, 8);
			printf("%d : %s\n", n, name);

			if (wad->files[n].lenData > 0)
			{
				ExtractFlat(wad, n);
			}
		}
	}
}

void LoadGamePalette(wad_file_t* wad)
{
	int index = FindWadEntry(wad, "PLAYPAL", 0);
	if (index != -1)
	{
		uint8_t* pal = (uint8_t*) GetWadFileData(wad, index);
		for (int n = 0; n < 256; n++)
		{
			doompalette[n] = 0xff000000 | (pal[n * 3 + 2] << 16) | (pal[n * 3 + 1] << 8) | (pal[n * 3]);
		}
	}
	//unsigned width, height;
	//lodepng_decode32_file((uint8_t**) &doompalette, &width, &height, "../tools/doompal.png");
}

int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		printf("Usage: %s [file.wad]", argv[0]);
		return 0;
	}

	//mapdata_t mapdata;
	//
	//const char* levelname = "E1M2";
	//LoadMapFromWad(&mapdata, argv[1], levelname);
	//
	//DumpMapToHeader(&mapdata, levelname);

	wad_file_t* wad = LoadWad("doom1.wad");

	if (wad)
	{
		LoadGamePalette(wad);
	
		mapdata_t mapdata;
		if (LoadMapDataFromWad(wad, "E1M1", &mapdata))
		{
			DumpMapToHeader(&mapdata, "E1M1");
		}

		ExtractPatches(wad);
		ExtractFlats(wad);
	}
	return 0;
}

#include <stdio.h>
#include "r_defs.h"
#include "DoomData.h"

#pragma warning(disable:4996)

void LoadMapFromWad(mapdata_t* map, const char* filename, const char* levelLabel);
void ExtractMapData(mapdata_t* src, map_t* dest);

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

int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		printf("Usage: %s [file.wad]", argv[0]);
		return 0;
	}

	mapdata_t mapdata;

	const char* levelname = "E1M1";
	LoadMapFromWad(&mapdata, argv[1], levelname);

	DumpMapToHeader(&mapdata, levelname);

	return 0;
}

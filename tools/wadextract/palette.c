#include <stdint.h>
#include <stdio.h>
#include <math.h>
#include "win32/wad.h"
#include "win32/lodepng.h"

#pragma warning(disable:4996)

#define VDPPALETTE_REDSFT           1
#define VDPPALETTE_GREENSFT         5
#define VDPPALETTE_BLUESFT          9

#define VDPPALETTE_REDMASK          0x000E
#define VDPPALETTE_GREENMASK        0x00E0
#define VDPPALETTE_BLUEMASK         0x0E00
#define VDPPALETTE_COLORMASK        0x0EEE

#define RGB24_TO_VDPCOLOR(color)    (((((color + 0x100000) < 0xFF0000 ? color + 0x100000 : 0xFF0000) >> (20)) & VDPPALETTE_REDMASK) | (((((color & 0xff00) + 0x1000) < 0xFF00 ? (color & 0xff00) + 0x1000 : 0xFF00) >> ((1 * 4) + 4)) & VDPPALETTE_GREENMASK) | (((((color & 0xff) + 0x10) < 0xFF ? (color & 0xff) + 0x10 : 0xFF) << 4) & VDPPALETTE_BLUEMASK))
#define RGB8_8_8_TO_VDPCOLOR(r, g, b) RGB24_TO_VDPCOLOR(((((b) << 0) & 0xFF) | (((g) & 0xFF) << 8) | (((r) & 0xFF) << 16)))

uint32_t doompalette[256];
uint32_t megadrivepalette[16];

uint32_t megadriveblendpalette[256];
bool blendpalettevalid[256];

void LoadGamePalette(wad_file_t* wad)
{
	int index = FindWadEntry(wad, "PLAYPAL", 0);
	if (index != -1)
	{
		uint8_t* pal = (uint8_t*)GetWadFileData(wad, index);
		for (int n = 0; n < 256; n++)
		{
			doompalette[n] = 0xff000000 | (pal[n * 3 + 2] << 16) | (pal[n * 3 + 1] << 8) | (pal[n * 3]);
		}
	}
}

#define MATCH_PAIR_THRESHOLD 300
//#define MATCH_PAIR_THRESHOLD 200
//#define MATCH_PAIR_THRESHOLD 80
#define USE_SIMPLE_COLOR_DISTANCE 1

float CalcDistance(int r1, int g1, int b1, int r2, int g2, int b2)
{
	int rmean = (r1 + r2) / 2;
	int r = r1 - r2;
	int g = g1 - g2;
	int b = b1 - b2;
#if USE_SIMPLE_COLOR_DISTANCE
	return (r * r) + (g * g) + (b * b);
#else
	return (float)(sqrt((((512 + rmean) * r * r) >> 8) + 4 * g * g + (((767 - rmean) * b * b) >> 8)));
#endif
}

uint8_t MatchPaletteColour(uint32_t input)
{
	if ((input & 0xff000000) == 0)
	{
		return 0;
	}

	int closest = -1;
	float closestdistance = 0;
	uint8_t* channelValues = (uint8_t*)(&input);

	for (int n = 1; n < 16; n++)
	{
		uint8_t* palValues = (uint8_t*)(&megadrivepalette[n]);
		float distance = CalcDistance(channelValues[0], channelValues[1], channelValues[2], palValues[0], palValues[1], palValues[2]);
		if (closest == -1 || distance < closestdistance)
		{
			closest = n;
			closestdistance = distance;
		}
	}

	return (uint8_t)closest;
}

uint8_t MatchBlendedColour(uint32_t input)
{
	int bestMixDistance = 0;
	int bestMixIndex = -1;
	uint8_t* mixColours = (uint8_t*)(megadriveblendpalette);
	uint8_t* palette = (uint8_t*)(megadrivepalette);

	if ((input & 0xff000000) == 0)
	{
		return 0;
	}

	for (int n = 0; n < 256; n++)
	{
		if (blendpalettevalid[n])
		{
			//float distance = 0;
			//for (int i = 0; i < 3; i++)
			//{
			//	uint8_t channelValue = ((uint8_t*)&input)[i];
			//	distance += (mixColours[n * 4 + i] - channelValue) * (mixColours[n * 4 + i] - channelValue);
			//}
			uint8_t* channelValues = (uint8_t*)(&input);
			float distance = CalcDistance(mixColours[n * 4], mixColours[n * 4 + 1], mixColours[n * 4 + 2], channelValues[0], channelValues[1], channelValues[2]);

			int pairDistance = 0;
			int pairFirst = n / 16;
			int pairSecond = n % 16;
			for (int i = 0; i < 3; i++)
			{
				int dist = palette[pairFirst * 4 + i] - palette[pairSecond * 4 + i];
				pairDistance += dist * dist;
			}
			if (pairDistance > MATCH_PAIR_THRESHOLD * MATCH_PAIR_THRESHOLD)
			{
				continue;
			}
			if (bestMixIndex == -1 || distance < bestMixDistance)
			{
				bestMixDistance = distance;
				bestMixIndex = n;
			}
		}
	}
	return (uint8_t)(bestMixIndex);
}

void GenerateBlendPalette()
{
	for (int x = 0; x < 16; x++)
	{
		for (int y = 0; y < 16; y++)
		{
			uint8_t* first = (uint8_t*)(&megadrivepalette[x]);
			uint8_t* second = (uint8_t*)(&megadrivepalette[y]);

			uint8_t red = (uint8_t)((first[0] + second[0]) / 2);
			uint8_t green = (uint8_t)((first[1] + second[1]) / 2);
			uint8_t blue = (uint8_t)((first[2] + second[2]) / 2);
			uint8_t alpha = (uint8_t)((first[3] + second[3]) / 2);

			uint32_t blended = (red) | (green << 8) | (blue << 16) | (alpha << 24);
			int index = y * 16 + x;

			if (x == 0 || y == 0)
			{
				megadriveblendpalette[index] = 0;
				blendpalettevalid[index] = false;
			}
			else
			{
				blendpalettevalid[index] = true;
				megadriveblendpalette[index] = blended;
			}
		}
	}

	//lodepng_encode32_file("blend.png", (uint8_t*) megadriveblendpalette, 16, 16);
}

void LoadMegadrivePalette(const char* imagepath, const char* outputpath, const char* varname)
{
	uint8_t* paletteColours;
	unsigned width, height;

	if (lodepng_decode32_file(&paletteColours, &width, &height, imagepath))
	{
		printf("Error opening palette file\n");
		return;
	}

	FILE* fs = fopen(outputpath, "wb");

	fprintf(fs, "#ifdef WIN32\n");
	fprintf(fs, "const uint32_t %s[] = {\n\t0xff000000, ", varname);
	for (unsigned n = 0; n < width; n++)
	{
		uint32_t colour = paletteColours[n * 4] | (paletteColours[n * 4 + 1] << 8) | (paletteColours[n * 4 + 2] << 16) | 0xff000000;
		fprintf(fs, "0x%x", colour);
		if (n < width - 1)
		{
			fprintf(fs, ", ");
		}
		else
		{
			fprintf(fs, "\n");
		}

		if (n + 1 < 16)
		{
			megadrivepalette[n + 1] = colour;
		}
	}
	fprintf(fs, "};\n");
	fprintf(fs, "#else\n");
	fprintf(fs, "const uint16_t %s[] = {\n\t0x2222, ", varname);
	for (unsigned n = 0; n < width; n++)
	{
		uint32_t colour = RGB8_8_8_TO_VDPCOLOR(paletteColours[n * 4], paletteColours[n * 4 + 1], paletteColours[n * 4 + 2]);
		fprintf(fs, "0x%x", colour);
		if (n < width - 1)
		{
			fprintf(fs, ", ");
		}
		else
		{
			fprintf(fs, "\n");
		}
	}
	fprintf(fs, "};\n");
	fprintf(fs, "#endif\n");

	fclose(fs);

	GenerateBlendPalette();
}


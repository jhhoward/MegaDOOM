#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include "win32/wad.h"
#include "win32/lodepng.h"
#include "palette.h"
#include <stdint.h>

#pragma warning(disable:4996)

typedef struct
{
	uint16_t width;
	uint16_t height;
	int16_t leftoffset;
	int16_t topoffset;
} patch_header_t;

int16_t LookupTexture(char name[8])
{
	return name[0] == '-' ? 0 : 1;
}

void DumpTextureToHeader(FILE* fs, const char* name, uint32_t* pixels, int width, int height)
{
	fprintf(fs, "const uint8_t %s_data[] = {\n\t", name);
	for (int x = 0; x < width; x++)
	{
		for (int y = 0; y < height; y++)
		{
			int index = y * width + x;
			int pixel = MatchBlendedColour(pixels[index]);
			if (index == width * height - 1)
			{
				fprintf(fs, "0x%x\n", pixel);
			}
			else
			{
				fprintf(fs, "0x%x, ", pixel);
			}
		}
	}
	fprintf(fs, "};\n\n");

	fprintf(fs, "const uint8_t *%s_columns[] = {\n\t", name);
	for (int x = 0; x < width; x++)
	{
		fprintf(fs, "&%s_data[%d]", name, x * height);
		if (x == width - 1)
		{
			fprintf(fs, "\n");
		}
		else
		{
			fprintf(fs, ", ");
		}
	}
	fprintf(fs, "};\n\n");

	fprintf(fs, "const texture_t %s = {\n\t", name);
	fprintf(fs, "%d, %d, %s_columns\n", width, height, name);
	fprintf(fs, "};\n\n");
}

const bool crosshatchdither = true;
const bool quantize = false;

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

	if (index == 1113)
	{
		FILE* fs = fopen("../src/generated/texture.inc.h", "w");
		DumpTextureToHeader(fs, "texture", pixels, header->width, header->height);
		fclose(fs);
	}

	if (quantize)
	{
		for (int y = 0; y < header->height; y++)
		{
			for (int x = 0; x < header->width; x += 2)
			{
				int pixIndex = y * header->width + x;

				if ((y & 1) && crosshatchdither)
				{
					uint8_t blended = MatchBlendedColour(pixels[pixIndex]);
					pixels[pixIndex] = megadrivepalette[blended >> 4];
					blended = MatchBlendedColour(pixels[pixIndex + 1]);
					pixels[pixIndex + 1] = megadrivepalette[blended & 0xf];
				}
				else
				{
					uint8_t blended = MatchBlendedColour(pixels[pixIndex]);
					pixels[pixIndex] = megadrivepalette[blended & 0xf];
					blended = MatchBlendedColour(pixels[pixIndex + 1]);
					pixels[pixIndex + 1] = megadrivepalette[blended >> 4];
				}
			}
		}
	}

	char filename[10];
	sprintf(filename, "%d.png", index);
	lodepng_encode32_file(filename, (uint8_t*)pixels, header->width, header->height);
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
		if (wad->files[n].name[0] == 'M' && wad->files[n].name[1] == '_')
		{
			ExtractPatch(wad, n);
		}
	}
}

void ExtractFlat(wad_file_t* wad, int index)
{
	uint8_t* data = (uint8_t*)GetWadFileData(wad, index);
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

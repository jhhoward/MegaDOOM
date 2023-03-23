#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include "win32/wad.h"
#include "win32/lodepng.h"
#include "palette.h"
#include "texture.h"
#include <stdint.h>

#pragma warning(disable:4996)

const bool dumppng = false;
const bool crosshatchdither = false;
const bool quantize = false;

typedef struct
{
	uint16_t width;
	uint16_t height;
	int16_t leftoffset;
	int16_t topoffset;
} patch_header_t;

typedef struct
{
	char name[8];
	int width, height;
	uint32_t* pixels;
} patch_t;

typedef struct
{
	char name[8];
} name_t;


typedef struct
{
	int16_t originx;
	int16_t originy;
	int16_t patchnum;
	int16_t stepdir;		// unused
	int16_t colormap;		// unused
} mappatch_t;

typedef struct
{
	char name[8];
	uint32_t masked;
	uint16_t width;
	uint16_t height;
	uint32_t columndirectory;  // unused
	uint16_t patchcount;
	mappatch_t patches[1];
} maptexture_t;

typedef struct
{
	char name[8];
	uint32_t* pixels;
	int width, height;
} compositetexture_t;

#define MAX_FLATS 256
#define MAX_PATCHES 256
#define MAX_COMPOSITE_TEXTURES 256

flat_t flats[MAX_FLATS];
name_t flatnames[MAX_FLATS];
int numflats = 0;

patch_t patches[MAX_PATCHES];
int numpatches = 0;

compositetexture_t compositetextures[MAX_COMPOSITE_TEXTURES];
int numcompositetextures = 0;

bool DoesNameMatch(char a[8], char b[8])
{
	return *((uint64_t*)a) == *((uint64_t*)b);
}

int16_t LookupTexture(char name[8])
{
	if (name[0] == '-')
		return 0;
	for (int n = 0; n < numcompositetextures; n++)
	{
		if (DoesNameMatch(name, compositetextures[n].name))
		{
			return n + 1;
		}
	}
	return 1;
}

int16_t LookupFlat(char name[8])
{
	for (int n = 0; n < numflats; n++)
	{
		if (DoesNameMatch(name, flatnames[n].name))
		{
			return n;
		}
	}

	return 0;
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

	fprintf(fs, "const walltexture_t %s = {\n\t", name);
	fprintf(fs, "%d, %d, %s_columns\n", width, height, name);
	fprintf(fs, "};\n\n");
}

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

	if (numpatches < MAX_PATCHES)
	{
		patch_t* patch = &patches[numpatches++];
		patch->width = header->width;
		patch->height = header->height;
		patch->pixels = pixels;
		memcpy(patch->name, file->name, 8);
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


	if (dumppng)
	{
		char filename[10];
		sprintf(filename, "%d.png", index);
		lodepng_encode32_file(filename, (uint8_t*)pixels, header->width, header->height);
	}
}

typedef struct
{
	int32_t count;
	name_t names[1];
} pnames_t;

void ExtractPatches(wad_file_t* wad)
{
	int pnamesIndex = FindWadEntry(wad, "PNAMES", 0);

	if (pnamesIndex != -1)
	{
		pnames_t* pnames = (pnames_t*)GetWadFileData(wad, pnamesIndex);

		for (int n = 0; n < pnames->count; n++)
		{
			int patchindex = FindWadEntry(wad, pnames->names[n].name, 0);
			if (patchindex == -1)
			{
				return;
			}
			ExtractPatch(wad, patchindex);
		}
	}

	//int start = FindWadEntry(wad, "P_START", 0);
	//int end = FindWadEntry(wad, "P_END", 0);
	//
	//if (start >= 0 && end > start)
	//{
	//	for (int n = start + 1; n < end; n++)
	//	{
	//		char name[9];
	//		name[8] = 0;
	//		memcpy(name, wad->files[n].name, 8);
	//		printf("%d : %s\n", n, name);
	//
	//		if (wad->files[n].lenData > 0)
	//		{
	//			ExtractPatch(wad, n);
	//		}
	//	}
	//}

	
	//for (int n = 0; n < wad->header->numFiles; n++)
	//{
	//	if (wad->files[n].name[0] == 'M' && wad->files[n].name[1] == '_')
	//	{
	//		ExtractPatch(wad, n);
	//	}
	//}
}

void ExtractFlat(wad_file_t* wad, int index)
{
	uint8_t* data = (uint8_t*)GetWadFileData(wad, index);
	uint32_t pixels[64 * 64];

	for (int n = 0; n < 64 * 64; n++)
	{
		pixels[n] = doompalette[data[n]];
	}

	float r = 0, g = 0, b = 0;
	for (int n = 0; n < 64 * 64; n++)
	{
		r += pixels[n] & 0xff;
		g += (pixels[n] >> 8) & 0xff;
		b += (pixels[n] >> 16) & 0xff;
	}

	r /= (64 * 64);
	g /= (64 * 64);
	b /= (64 * 64);

	if (numflats < MAX_FLATS)
	{
		memcpy(flatnames[numflats].name, wad->files[index].name, 8);
		for (int n = 0; n < 8; n++)
		{
			float alpha = (n + 1) / 8.0f;
			alpha *= 1.1f;
			int rlit = (int)(r * alpha);
			int glit = (int)(g * alpha);
			int blit = (int)(b * alpha);
			if (rlit > 255)
				rlit = 255;
			if (glit > 255)
				glit = 255;
			if (blit > 255)
				blit = 255;

			uint32_t averagelit = 0xff000000 | (rlit) | (glit << 8) | (blit << 16);

			flats[numflats].colour[n] = MatchBlendedColour(averagelit);
		}
		numflats++;
	}


	if (dumppng)
	{
		char filename[32];
		sprintf(filename, "%d.png", index);
		lodepng_encode32_file(filename, (uint8_t*)pixels, 64, 64);

		uint32_t average = 0xff000000 | ((int)r) | ((int)g << 8) | ((int)b << 16);

		for (int n = 0; n < 64 * 64; n++)
		{
			pixels[n] = average;
		}

		sprintf(filename, "%d_avg.png", index);
		lodepng_encode32_file(filename, (uint8_t*)pixels, 64, 64);
	}
}

void WriteFlatsToHeader()
{
	FILE* fs = fopen("../src/generated/flats.inc.h", "w");

	fprintf(fs, "const flat_t flats[] = {\n");
	for (int n = 0; n < numflats; n++)
	{
		fprintf(fs, "\t{ {");
		for (int i = 0; i < 8; i++)
		{
			fprintf(fs, "0x%x, ", flats[n].colour[i]);
		}
		fprintf(fs, "} },\n");
	}
	fprintf(fs, "};\n");

	fclose(fs);
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

	WriteFlatsToHeader();
}

void WriteTexturesToHeader()
{
	FILE* fs = fopen("../src/generated/textures.inc.h", "w");
	char texname[9];
	texname[8] = '\0';

	for (int n = 0; n < numcompositetextures; n++)
	{
		compositetexture_t* compositetexture = &compositetextures[n];
		memcpy(texname, compositetexture->name, 8);
		fprintf(fs, "const uint8_t texturedata_%s[] = {\n", texname);

		for (int l = 0; l < 2; l++)
		{
			float alpha = (l + 1) / 2.0f;
			for (int x = 0; x < compositetexture->width; x++)
			{
				for (int y = 0; y < compositetexture->height; y++)
				{
					uint32_t pixel = compositetexture->pixels[y * compositetexture->width + x];
					int r = pixel & 0xff;
					int g = (pixel >> 8) & 0xff;
					int b = (pixel >> 16) & 0xff;
					int rlit = (int)(r * alpha);
					int glit = (int)(g * alpha);
					int blit = (int)(b * alpha);
					if (rlit > 255)
						rlit = 255;
					if (glit > 255)
						glit = 255;
					if (blit > 255)
						blit = 255;

					uint32_t averagelit = 0xff000000 | (rlit) | (glit << 8) | (blit << 16);

					int palettedPixel = MatchBlendedColour(averagelit);
					fprintf(fs, "0x%x,", palettedPixel);
				}
			}
		}

		fprintf(fs, "};\n");
		fprintf(fs, "const uint8_t* const texturecolumns_%s[] = {\n", texname);
		for (int x = 0; x < compositetexture->width * 2; x++)
		{
			fprintf(fs, "&texturedata_%s[%d], ", texname, x * compositetexture->height);
		}
		fprintf(fs, "};\n");
	}

	fprintf(fs, "const walltexture_t walltextures[] = { { 0, 0, 0 },\n");
	for (int n = 0; n < numcompositetextures; n++)
	{
		memcpy(texname, compositetextures[n].name, 8);
		fprintf(fs, "\t{ %d, %d, texturecolumns_%s },\n", compositetextures[n].width, compositetextures[n].height, texname);
	}
	fprintf(fs, "};\n");

	fclose(fs);
}

void ExtractTextures(wad_file_t* wad)
{
	int texture1 = FindWadEntry(wad, "TEXTURE1", 0);
	if (texture1 >= 0)
	{
		wad_file_entry_t* entry = &wad->files[texture1];
		uint8_t* data = wad->data + entry->offData;
		uint32_t texturecount = *((uint32_t*)data);
		printf("Num textures: %d\n", texturecount);
		uint32_t* offsets = ((uint32_t*)data) + 1;

		for (int n = 0; n < texturecount; n++)
		{
			if (numcompositetextures >= MAX_COMPOSITE_TEXTURES)
				break;

			maptexture_t* maptexture = (maptexture_t*)(data + offsets[n]);
			printf("%s\n", maptexture->name);

			compositetexture_t* compositetexture = &compositetextures[numcompositetextures++];
			compositetexture->width = maptexture->width;
			compositetexture->height = maptexture->height;
			compositetexture->pixels = (uint32_t*) malloc(compositetexture->width * compositetexture->height * sizeof(uint32_t));
			memset(compositetexture->pixels, 0, compositetexture->width * compositetexture->height * sizeof(uint32_t));
			memcpy(compositetexture->name, maptexture->name, 8);

			mappatch_t* texturepatches = maptexture->patches;

			for (int p = 0; p < maptexture->patchcount; p++)
			{ 
				mappatch_t* texturepatch = &texturepatches[p];

				if (texturepatch->patchnum < numpatches)
				{
					patch_t* patch = &patches[texturepatch->patchnum];

					for (int j = 0; j < patch->height; j++)
					{
						for (int i = 0; i < patch->width; i++)
						{
							int outx = texturepatch->originx + i;
							int outy = texturepatch->originy + j;
							if (outx >= compositetexture->width || outy >= compositetexture->height || outx < 0 || outy < 0)
							{
								continue;
							}
							uint32_t pixel = patch->pixels[j * patch->width + i];

							if (pixel)
							{
								compositetexture->pixels[outy * compositetexture->width + outx] = pixel;
							}
						}
					}
				}
			}

			if (dumppng)
			{
				if (quantize)
				{
					for (int y = 0; y < compositetexture->height; y++)
					{
						for (int x = 0; x < compositetexture->width; x += 2)
						{
							int pixIndex = y * compositetexture->width + x;

							if ((y & 1) && crosshatchdither)
							{
								uint8_t blended = MatchBlendedColour(compositetexture->pixels[pixIndex]);
								compositetexture->pixels[pixIndex] = megadrivepalette[blended >> 4];
								blended = MatchBlendedColour(compositetexture->pixels[pixIndex + 1]);
								compositetexture->pixels[pixIndex + 1] = megadrivepalette[blended & 0xf];
							}
							else
							{
								uint8_t blended = MatchBlendedColour(compositetexture->pixels[pixIndex]);
								compositetexture->pixels[pixIndex] = megadrivepalette[blended & 0xf];
								blended = MatchBlendedColour(compositetexture->pixels[pixIndex + 1]);
								compositetexture->pixels[pixIndex + 1] = megadrivepalette[blended >> 4];
							}
						}
					}
				}

				char name[9];
				memcpy(name, maptexture->name, 8);
				name[8] = '\0';
				char filename[32];
				sprintf(filename, "%s.png", name);
				lodepng_encode32_file(filename, (uint8_t*)compositetexture->pixels, compositetexture->width, compositetexture->height);
			}
		}
	}

	WriteTexturesToHeader();
}


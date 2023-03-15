#include <stdio.h>
#include <vector>
#include "lodepng.cpp"

using namespace std;

#define VDPPALETTE_REDSFT           1
#define VDPPALETTE_GREENSFT         5
#define VDPPALETTE_BLUESFT          9

#define VDPPALETTE_REDMASK          0x000E
#define VDPPALETTE_GREENMASK        0x00E0
#define VDPPALETTE_BLUEMASK         0x0E00
#define VDPPALETTE_COLORMASK        0x0EEE

#define RGB24_TO_VDPCOLOR(color)    (((((color + 0x100000) < 0xFF0000 ? color + 0x100000 : 0xFF0000) >> (20)) & VDPPALETTE_REDMASK) | (((((color & 0xff00) + 0x1000) < 0xFF00 ? (color & 0xff00) + 0x1000 : 0xFF00) >> ((1 * 4) + 4)) & VDPPALETTE_GREENMASK) | (((((color & 0xff) + 0x10) < 0xFF ? (color & 0xff) + 0x10 : 0xFF) << 4) & VDPPALETTE_BLUEMASK))
#define RGB8_8_8_TO_VDPCOLOR(r, g, b) RGB24_TO_VDPCOLOR(((((b) << 0) & 0xFF) | (((g) & 0xFF) << 8) | (((r) & 0xFF) << 16)))


int main(int argc, char* argv[])
{
	vector<uint8_t> paletteColours;
	unsigned width, height;

	if(lodepng::decode(paletteColours, width, height, "megadoompal.png"))
	{
		printf("Error opening palette file\n");
		return 0;
	}

	FILE* fs = fopen("palette.inc", "wb");
	
	fprintf(fs, "#ifdef WIN32\n");
	fprintf(fs, "const uint32_t gamePalette[] = {\n\t0xff000000, ");
	for(unsigned n = 0; n < width; n++)
	{
		uint32_t colour = paletteColours[n * 4] | (paletteColours[n * 4 + 1] << 8) | (paletteColours[n * 4 + 2] << 16) | 0xff000000;
		fprintf(fs, "0x%x", colour);
		if(n < width - 1)
		{
			fprintf(fs, ", ");
		}
		else
		{
			fprintf(fs, "\n");
		}
	}
	fprintf(fs, "};\n");
	fprintf(fs, "#else\n");
	fprintf(fs, "const uint16_t gamePalette[] = {\n\t0x0, ");
	for(unsigned n = 0; n < width; n++)
	{
		uint32_t colour = RGB8_8_8_TO_VDPCOLOR(paletteColours[n * 4], paletteColours[n * 4 + 1], paletteColours[n * 4 + 2]);
		fprintf(fs, "0x%x", colour);
		if(n < width - 1)
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

	return 0;
}

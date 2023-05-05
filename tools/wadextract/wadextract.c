#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include "win32/lodepng.h"
#include "win32/wad.h"
#include "texture.h"
#include "palette.h"
#include "level.h"

#pragma warning(disable:4996)

bool extractlevels = true;
bool extractsingletexture = true;
bool extracttextures = true;
bool writeskybox = true;

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

	wad_file_t* iwad = LoadWad("doom1.wad");
	wad_file_t* pwad = LoadWad("test2.wad");
//	wad_file_t* pwad = LoadWad("jaguartc_fix.wad");
	
	if (iwad && pwad)
	{
		LoadGamePalette(iwad);
		LoadMegadrivePalette("../assets/megadoompal.png", "../src/generated/palette.inc.h", "gamePalette");
		//LoadMegadrivePalette("../tools/megadoompal7.png", "../src/generated/palette.inc.h", "gamePalette");

		if (extracttextures)
		{
			ExtractPatches(iwad);
			ExtractFlats(iwad);
			ExtractTextures(iwad);
		}
		else if (extractlevels)
		{
			ExtractFlatsInfoOnly(iwad);
			ExtractTextureInfoOnly(iwad);
		}

		if (extractlevels)
		{
			mapdata_t mapdata;
			char levelname[9];
			memset(levelname, 0, 9);
			for (int n = 1; n < 10; n++)
			{
				sprintf(levelname, "E1M%d", n);
				if (LoadMapDataFromWad(iwad, levelname, &mapdata))
				{
					DumpMapToHeader(&mapdata, levelname);
				}
			}
		}

		if (writeskybox)
		{
			LoadMegadrivePalette("../assets/skypal.png", "../src/generated/skypalette.inc.h", "skyPalette");
			WriteSkybox("../assets/SKY1MD.png", "../src/generated/skybox.inc.h");
		}
	}
	return 0;
}

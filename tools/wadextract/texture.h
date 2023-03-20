#pragma once
#include "win32/wad.h"

int16_t LookupTexture(char name[8]);
void ExtractPatches(wad_file_t* wad);
void ExtractFlats(wad_file_t* wad);

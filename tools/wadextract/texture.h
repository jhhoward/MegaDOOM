#pragma once
#include "win32/wad.h"


int16_t LookupTexture(char name[8]);
int16_t LookupFlat(char name[8]);
void ExtractPatches(wad_file_t* wad);
void ExtractFlats(wad_file_t* wad);
void ExtractFlatsInfoOnly(wad_file_t* wad);
void ExtractTextures(wad_file_t* wad);
void ExtractTextureInfoOnly(wad_file_t* wad);

void WriteSkybox(const char* imagepath, const char* outputpath);


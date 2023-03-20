#pragma once
#include "win32/wad.h"

uint32_t doompalette[256];
uint32_t megadriveblendpalette[256];
uint32_t megadrivepalette[16];

void LoadGamePalette(wad_file_t* wad);
void LoadMegadrivePalette();
void GenerateBlendPalette();
void InitPalettes(wad_file_t* wad);
uint8_t MatchBlendedColour(uint32_t input);

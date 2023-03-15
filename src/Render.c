#include "doomtypes.h"
#include <stdio.h>
#include "Map.h"
#include "r_local.h"

extern int16_t floorClip[256];
extern int16_t ceilingClip[256];
extern int16_t columnsToFill;

void RenderAll()
{
    for (int n = 0; n < 256; n++)
    {
        floorClip[n] = 224;
        ceilingClip[n] = -1;
    }

    columnsToFill = 256;
    R_RenderBSPNode(currentlevel->rootnode);
}
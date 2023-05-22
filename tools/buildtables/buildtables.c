#include <stdint.h>
#include <stdbool.h>
#include "tables.h"
#include "r_defs.h"
#include <stdio.h>
#include <math.h>
#pragma warning(disable:4996)

#define M_PI 3.141592654

#define FRAMEBUFFER_WIDTH SCREENWIDTH
#define FRAMEBUFFER_WIDTH_TILES (FRAMEBUFFER_WIDTH / 4)
#define FRAMEBUFFER_HEIGHT (SCREENHEIGHT * 2)
#define FRAMEBUFFER_HEIGHT_TILES (FRAMEBUFFER_HEIGHT / 8)
#define FRAMEBUFFER_TILE_BYTES (4 * 8)

#define SCREEN_LAYOUT_WIDTH_TILES (256 / 8)
#define SCREEN_LAYOUT_HEIGHT_TILES (224 / 8)

#define SKY_WIDTH_TILES (FRAMEBUFFER_WIDTH_TILES)
#define SKY_HEIGHT 96
#define SKY_LAYOUT_WIDTH_TILES (SKY_WIDTH_TILES  * 2)
#define SKY_LAYOUT_HEIGHT_TILES (SKY_HEIGHT / 8)

#define TILE_USER_INDEX 16
#define FRAMEBUFFER_START_TILE_INDEX (TILE_USER_INDEX + 1)
#define SKY_START_TILE_INDEX (FRAMEBUFFER_START_TILE_INDEX + FRAMEBUFFER_WIDTH_TILES * FRAMEBUFFER_HEIGHT_TILES)

int main()
{
    int	i;
    fixed_t	t;
    float	a;

    FILE* fs = fopen("tables.inc.h", "w");

    fprintf(fs, "const fixed16_t finesine[%d] = {\n\t", 5 * FINEANGLES / 4);

    // finesine table
    for (i = 0; i < 5 * FINEANGLES / 4; i++)
    {
        a = (float)((i + 0.5) * M_PI * 2 / FINEANGLES);
        t = (fixed_t)(FRACUNIT * sin(a));
        fprintf(fs, "%d, ", t);
    }

    fprintf(fs, "\n};\n");

    fprintf(fs, "const fixed_t finetangent[%d] = {\n\t", FINEANGLES / 2);
    // viewangle tangent table
    for (i = 0; i < FINEANGLES / 2; i++)
    {
        a = (i - FINEANGLES / 4 + 0.5) * M_PI * 2 / FINEANGLES;
        float fv = FRACUNIT * tan(a);
        t = fv;
        // t = (int)(t / 256) * 256;
        fprintf(fs, "%d, ", t);
    }
    fprintf(fs, "\n};\n");

    //
    // slope (tangent) to angle lookup
    //
    fprintf(fs, "const angle_t tantoangle[%d] = {\n\t", SLOPERANGE + 1);
    for (i = 0; i <= SLOPERANGE; i++)
    {
        float f = atan((float)i / SLOPERANGE) / (3.141592657 * 2);
        t = 0xffff * f;
        fprintf(fs, "%d, ", t);
    }
    fprintf(fs, "\n};\n");

    // point to angle lookup
    fprintf(fs, "const angle_t pointtoangle[ANGLE_LOOKUP_TABLE_DIMENSION * ANGLE_LOOKUP_TABLE_DIMENSION] = {\n");
    for (int x = 0; x < ANGLE_LOOKUP_TABLE_DIMENSION; x++)
    {
        fprintf(fs, "\t ");
        for (int y = 0; y < ANGLE_LOOKUP_TABLE_DIMENSION; y++)
        {
            float f = atan2(y, x) / (3.141592657 * 2);
            t = 0xffff * f;
            fprintf(fs, "%d, ", t);
        }
        fprintf(fs, "\n");
    }
    fprintf(fs, "};\n");

    fprintf(fs, "const fixed_t scaledividetable[1024] = {\n\t");
    for (i = 0; i < 1024; i++)
    {
        fprintf(fs, "%d, ", i == 0 ? 0xffff : 0xfffffu / i);
    }
    fprintf(fs, "\n};\n");

#if 0
    // distancescale
    fprintf(fs, "const int16_t distancescale[] = {\n");
    for (int n = 0; n < 2048; n++)
    {
        int result = n == 0 ? 0 : ((SCREENWIDTH / 2) << 10) / n;
        fprintf(fs, "%d", result);
        if (n == 2047)
        {
            fprintf(fs, "\n");
        }
        else
        {
            fprintf(fs, ", ");
        }
    }
    fprintf(fs, "};\n\n");

    // reciprocal table
    fprintf(fs, "const int16_t reciprocal[] = {\n");
    for (int n = 0; n < 1024; n++)
    {
        int result = n == 0 ? 0 : 1024 / n;
        fprintf(fs, "%d", result);
        if (n == 1023)
        {
            fprintf(fs, "\n");
        }
        else
        {
            fprintf(fs, ", ");
        }
    }
    fprintf(fs, "};\n\n");
#endif

    fclose(fs);

    // framebuffer tables
    fs = fopen("framebuffer.inc.h", "w");

    fprintf(fs, "const u16 framebufferx[] = {\n");

    for (int x = 0; x < FRAMEBUFFER_WIDTH; x++)
    {
        int ptr = 0;
        ptr += (x >> 2) * (FRAMEBUFFER_HEIGHT_TILES * FRAMEBUFFER_TILE_BYTES);
        ptr += (x & 3);
        fprintf(fs, "%d", ptr);
        if (x == 255)
        {
            fprintf(fs, "\n");
        }
        else
        {
            fprintf(fs, ", ");
        }
    }
    fprintf(fs, "};\n\n");

    fprintf(fs, "const u16 screenLayoutTiles[] = {\n");
    for (int y = 0; y < SCREEN_LAYOUT_HEIGHT_TILES; y++)
    {
        for (int x = 0; x < SCREEN_LAYOUT_WIDTH_TILES; x++)
        {
            int i = x - (SCREEN_LAYOUT_WIDTH_TILES - FRAMEBUFFER_WIDTH_TILES) / 2;
            int j = y;

            if (i >= 0 && j >= 0 && i < FRAMEBUFFER_WIDTH_TILES && j < FRAMEBUFFER_HEIGHT_TILES)
            {
                int index = FRAMEBUFFER_START_TILE_INDEX + i * FRAMEBUFFER_HEIGHT_TILES + j;
                fprintf(fs, "%d, ", index);
            }
            else
            {
                int index = TILE_USER_INDEX;
                fprintf(fs, "%d, ", index);
            }
        }
    }
    fprintf(fs, "\n};\n\n");

    // Skybox tile map
    fprintf(fs, "const u16 skyLayoutTiles[] = {\n");
    for (int y = 0; y < SKY_LAYOUT_HEIGHT_TILES; y++)
    {
        for (int x = 0; x < SKY_LAYOUT_WIDTH_TILES; x++)
        {
            int i = x % (SKY_WIDTH_TILES);
            int j = y;
            
            int index = SKY_START_TILE_INDEX + j * SKY_WIDTH_TILES + i;

            // Set palette 1
            index |= (1 << 13);

            fprintf(fs, "%d, ", index);
        }
    }
    fprintf(fs, "\n};\n\n");
    fprintf(fs, "void R_DrawVLine(void)\n");
    fprintf(fs, "{\n");
    fprintf(fs, "\tint16_t count = dc_yh - dc_yl;\n");
    fprintf(fs, "\tif(count < 0) return;\n");
    fprintf(fs, "\tu8* ptr = framebuffer + framebufferx[dc_x];\n");
    fprintf(fs, "\tptr += (dc_yl << 3);\n");
    fprintf(fs, "\tswitch(count) {");
    for (int y = FRAMEBUFFER_HEIGHT / 2; y >= 0; y--)
    {
        fprintf(fs, "\tcase %d:\n", y);
        fprintf(fs, "\tptr[%d] = dc_col;\n", (y) * 8);
        fprintf(fs, "\tptr[%d] = dc_col;\n", (y) * 8 + 4);
    }
    fprintf(fs, "\tbreak;\n");
    fprintf(fs, "\t}\n");
    fprintf(fs, "}\n\n");
    
    fprintf(fs, "void R_DrawColumn(void)\n");
    fprintf(fs, "{\n");
    fprintf(fs, "\tint16_t count;\n");
    fprintf(fs, "\tbyte texel;\n");
    fprintf(fs, "\tfixed_t frac;\n");
    fprintf(fs, "\tfixed_t fracstep;\n");
    fprintf(fs, "\tcount = dc_yh - dc_yl;\n");
    fprintf(fs, "\tif (count < 0) return;\n");
    fprintf(fs, "\tu8* dest = framebuffer + framebufferx[dc_x];\n");
    fprintf(fs, "\tdest += (dc_yl << 3);\n");
    fprintf(fs, "\tfracstep = dc_iscale;\n");
    fprintf(fs, "\tfrac = dc_texturemid + (dc_yl - centery) * fracstep;\n");

    fprintf(fs, "\tswitch(count) {\n");
    for (int y = FRAMEBUFFER_HEIGHT / 2; y >= 0; y--)
    {
        fprintf(fs, "\tcase %d:\n", y);
//        fprintf(fs, "\t\ttexel = dc_source[(frac >> FRACBITS) & 127];\n");
        fprintf(fs, "\t\ttexel = dc_source[(frac >> FRACBITS)];\n");
        fprintf(fs, "\t\t*dest = texel;\n");
        fprintf(fs, "\t\tdest += 4;\n");
        fprintf(fs, "\t\t*dest = texel;\n");
        if (y > 0)
        {
            fprintf(fs, "\t\tdest += 4;\n");
            fprintf(fs, "\t\tfrac += fracstep;\n");
        }
    }
    fprintf(fs, "\t}\n");
    fprintf(fs, "}\n\n");

    fclose(fs);

	return 0;
}

#include "doomtypes.h"
#include "tables.h"
#include "r_defs.h"
#include <stdio.h>
#include <math.h>
#pragma warning(disable:4996)

#define M_PI 3.141592654
#define FRACBITS		8
#define FRACUNIT		(1<<FRACBITS)

#define FRAMEBUFFER_WIDTH VIEWPORT_WIDTH
#define FRAMEBUFFER_HEIGHT VIEWPORT_HEIGHT
#define FRAMEBUFFER_HEIGHT_TILES (FRAMEBUFFER_HEIGHT / 8)
#define FRAMEBUFFER_TILE_BYTES (4 * 8)

int main()
{
    int	i;
    int16_t	t;
    float	a;

    FILE* fs = fopen("tables.inc.h", "w");

    fprintf(fs, "const int16_t finesine[10240] = {\n\t");

    // finesine table
    for (i = 0; i < 5 * FINEANGLES / 4; i++)
    {
        a = (float)((i + 0.5) * M_PI * 2 / FINEANGLES);
        t = (int16_t)(FRACUNIT * sin(a));
        fprintf(fs, "%d, ", t);
    }

    fprintf(fs, "\n};\n");

    // distancescale
    fprintf(fs, "const int16_t distancescale[] = {\n");
    for (int n = 0; n < 2048; n++)
    {
        int result = n == 0 ? 0 : (VIEWPORT_HALF_WIDTH << 10) / n;
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

    fprintf(fs, "void VLine(int x, int y, int count, uint8_t colour)\n");
    fprintf(fs, "{\n");
    fprintf(fs, "\tu8* ptr = framebuffer + framebufferx[x];\n");
    fprintf(fs, "\tptr += (y << 2);\n");
    fprintf(fs, "\tswitch(count) {");
    for (int y = FRAMEBUFFER_HEIGHT; y > 0; y--)
    {
        fprintf(fs, "\tcase %d:\n", y);
        fprintf(fs, "\tptr[%d] = colour;\n", (y - 1) * 4);
    }
    fprintf(fs, "\tbreak;\n");
    fprintf(fs, "\t}\n");
    fprintf(fs, "}\n");

    fclose(fs);

	return 0;
}

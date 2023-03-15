#include "doomtypes.h"
#include "tables.h"
#include <stdio.h>
#include <math.h>
#pragma warning(disable:4996)

#define M_PI 3.141592654
#define FRACBITS		8
#define FRACUNIT		(1<<FRACBITS)

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

    fclose(fs);

	return 0;
}

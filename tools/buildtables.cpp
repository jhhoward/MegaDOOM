#include <stdio.h>
#include <math.h>

#define M_PI 3.141592654

int main()
{
	// finesine
	for(int n = 0; n < /*10240*/ 20; n++)
	{
		double angle = ((n + 0.5) * M_PI * 2) / 8192;
		int result = (int)(sin(angle) * 65536);
		result = (result >> 2) << 2;
		printf("%d, ", result);
	}

	return 0;
}

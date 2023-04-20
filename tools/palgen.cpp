#include <Windows.h>
#include <stdio.h>
#include <stdint.h>
#include <algorithm>
#include "lodepng.cpp"
#pragma comment(lib,"User32.lib")

using namespace std;

#define USE_SIMPLE_COLOR_DISTANCE 0

#define NUM_OUTPUT_COLOURS 15
//#define MATCH_PAIR_THRESHOLD 220
//#define MATCH_PAIR_THRESHOLD 200
//#define MATCH_PAIR_THRESHOLD 80
#define MATCH_PAIR_THRESHOLD 120
#define USE_GAMMA_CORRECTION 0

bool forceBlackAndWhite = true;

uint8_t GammaToLinear(uint8_t x)
{
	return (uint8_t)(255.0 * pow((x / 255.0), 2.2));
}

uint8_t LinearToGamma(uint8_t x)
{
	return (uint8_t)(255.0 * pow((x / 255.0), 1.0 / 2.2));
}

void GammaToLinear(vector<uint8_t> colours)
{
#if USE_GAMMA_CORRECTION
	int numColours = colours.size() / 4;
	for(int n = 0; n < numColours; n++)
	{
		for(int c = 0; c < 3; c++)
		{
			colours[n * 4 + c] = GammaToLinear(colours[n * 4 + c]);
		}
	}
#endif
}

void LinearToGamma(vector<uint8_t> colours)
{
#if USE_GAMMA_CORRECTION
	int numColours = colours.size() / 4;
	for(int n = 0; n < numColours; n++)
	{
		for(int c = 0; c < 3; c++)
		{
			colours[n * 4 + c] = LinearToGamma(colours[n * 4 + c]);
		}
	}
#endif
}

float CalcDistance(int r1, int g1, int b1, int r2, int g2, int b2)
{
	int rmean = (r1 + r2) / 2;
	int r = r1 - r2;
	int g = g1 - g2;
	int b = b1 - b2;
#if USE_SIMPLE_COLOR_DISTANCE
	return r * r + g * g + b * b;
#else
	return (float)(sqrt((((512+rmean)*r*r)>>8) + 4*g*g + (((767-rmean)*b*b)>>8)));
#endif
}

void GenerateMixColours(vector<uint8_t>& mixColours, vector<uint8_t>& paletteColours, vector<int>& outputColourIndices)
{
	mixColours.clear();
	
	for(int x = 0; x < outputColourIndices.size(); x++)
	{
		for(int y = 0; y < outputColourIndices.size(); y++)
		{
			int first = outputColourIndices[x];
			int second = outputColourIndices[y];
			mixColours.push_back((uint8_t)((paletteColours[first * 4 + 0] + paletteColours[second * 4 + 0]) / 2));
			mixColours.push_back((uint8_t)((paletteColours[first * 4 + 1] + paletteColours[second * 4 + 1]) / 2));
			mixColours.push_back((uint8_t)((paletteColours[first * 4 + 2] + paletteColours[second * 4 + 2]) / 2));
			mixColours.push_back(255);
		}
	}
}

float CalculateDistance(vector<uint8_t>& mixColours, vector<uint8_t>& targetPaletteColours, vector<uint8_t>& paletteColours, vector<int>& colourIndices)
{
	int numTargetPaletteColours = targetPaletteColours.size() / 4;
	int numMixColours = mixColours.size() / 4;
	float totalDistance = 0;
	
	for(int n = 0; n < numTargetPaletteColours; n++)
	{
		float closest = -1;
		
		for(int c = 0; c < numMixColours; c++)
		{
			//float distance = 0;
			//for(int i = 0; i < 3; i++)
			//{
			//	distance += (mixColours[c * 4 + i] - targetPaletteColours[n * 4 + i]) * (mixColours[c * 4 + i] - targetPaletteColours[n * 4 + i]);
			//}
			float distance = CalcDistance(mixColours[c * 4], mixColours[c * 4 + 1], mixColours[c * 4 + 2], targetPaletteColours[n * 4], targetPaletteColours[n * 4 + 1], targetPaletteColours[n * 4 + 2]);
			
			int pairDistance = 0;
			int pairFirst = colourIndices[c / colourIndices.size()];
			int pairSecond = colourIndices[c % colourIndices.size()];
			for(int i = 0; i < 3; i++)
			{
				int dist = paletteColours[pairFirst * 4 + i] - paletteColours[pairSecond * 4 + i];
				pairDistance += dist * dist;
			}
			if(pairDistance > MATCH_PAIR_THRESHOLD * MATCH_PAIR_THRESHOLD)
			{
				continue;
			}			
			
			if(closest < 0 || distance < closest)
			{
				closest = distance;
			}
		}
		totalDistance += closest;
	}
	
	return totalDistance;
}

int main(int argc, char* argv[])
{
	vector<uint8_t> paletteColours;
	unsigned width, height;
	
	if(lodepng::decode(paletteColours, width, height, "megapal.png"))
	{
		printf("Error opening palette file\n");
		return 0;
	}
	GammaToLinear(paletteColours);
	
	int numPaletteColours = paletteColours.size() / 4;

	vector<uint8_t> targetPaletteColours;
	
	if(argc == 2)
	{
		vector<uint8_t> referencePaletteColours;
		
		if(lodepng::decode(referencePaletteColours, width, height, argv[1]))
		{
			printf("Error opening palette file\n");
			return 0;
		}
		
		for(int n = 0; n < referencePaletteColours.size() / 4; n++)
		{
			uint8_t r = referencePaletteColours[n * 4];
			uint8_t g = referencePaletteColours[n * 4 + 1];
			uint8_t b = referencePaletteColours[n * 4 + 2];
			uint8_t a = referencePaletteColours[n * 4 + 3];
			
			if(a < 255)
				continue;
			
			if(r == 0 && g == 255 && b == 255)
				continue;
			
			bool found = false;
			
			for(int i = 0; i < targetPaletteColours.size() / 4; i++)
			{
				if(targetPaletteColours[i * 4] == r && targetPaletteColours[i * 4 + 1] == g && targetPaletteColours[i * 4 + 2] == b)
				{
					found = true;
					break;
				}
			}
			
			if(!found)
			{
				targetPaletteColours.push_back(r);
				targetPaletteColours.push_back(g);
				targetPaletteColours.push_back(b);
				targetPaletteColours.push_back(255);
			}
		}
		width = targetPaletteColours.size() / 4;
		height = 1;
		
		printf("Num input colours: %d\n", width);
	}
	else
	{
		if(lodepng::decode(targetPaletteColours, width, height, "pal.png"))
		{
			printf("Error opening palette file\n");
			return 0;
		}
	}
	GammaToLinear(targetPaletteColours);
	
	vector<int> bestColourIndices;

	srand(time(NULL));
	
	for(int n = 0; n < NUM_OUTPUT_COLOURS; n++)
	{
		bestColourIndices.push_back(rand() % numPaletteColours);
	}
	
	if(forceBlackAndWhite)
	{
		bestColourIndices[0] = 0;
		bestColourIndices[1] = numPaletteColours - 1;
	}
	
	vector<uint8_t> mixColours;
	int iterations = 1000000;


	GenerateMixColours(mixColours, paletteColours, bestColourIndices);

	float bestDistance = CalculateDistance(mixColours, targetPaletteColours, paletteColours, bestColourIndices);
	
	int firstColourIterator = forceBlackAndWhite ? 2 : 0;
	
	int colourIterator = firstColourIterator;
	int sourceIterator = 0;
	bool hasChanges = false;
	
	while(1)
	{
		if(sourceIterator < numPaletteColours)
		{
			int oldMapping = bestColourIndices[colourIterator];
			bestColourIndices[colourIterator] = sourceIterator;
			
			GenerateMixColours(mixColours, paletteColours, bestColourIndices);
			float distance = CalculateDistance(mixColours, targetPaletteColours, paletteColours, bestColourIndices);
			
			if(distance < bestDistance)
			{
				bestDistance = distance;
				hasChanges = true;

				printf("%f\n", bestDistance);
			}
			else
			{
				bestColourIndices[colourIterator] = oldMapping;
			}
		}
		else
		{
			colourIterator++;
			sourceIterator = 0;
			if(colourIterator == NUM_OUTPUT_COLOURS)
			{
				if(hasChanges)
				{
					colourIterator = firstColourIterator;
					hasChanges = false;
					printf(".\n");
				}
				else
				{
					break;
				}
			}
		}
		
		sourceIterator++;

		if (GetKeyState(' ') & 0x8000)
		{
			break;
		}
	}
	
	/*while(iterations > 0)
	{
		vector<int> testColourIndices(bestColourIndices);
		int numChanges = (rand() % (NUM_OUTPUT_COLOURS - 1)) + 1;
		numChanges = 1;
		
		for(int i = 0; i < numChanges; i++)
		{
			int pick = rand() % NUM_OUTPUT_COLOURS;
			int change = rand() % numPaletteColours;
			bool canChange = true;
			
			for(int n = 0; n < testColourIndices.size(); n++)
			{
				if(testColourIndices[n] == change)
				{
					canChange = false;
					break;
				}
			}
			
			if(!canChange)
			{
				continue;
			}
			testColourIndices[pick] = change;
		}
		
		
		GenerateMixColours(mixColours, paletteColours, testColourIndices);
		float distance = CalculateDistance(mixColours, targetPaletteColours, paletteColours, testColourIndices);
		
		if(distance < bestDistance)
		{
			bestColourIndices = testColourIndices;
			bestDistance = distance;

			printf("%f [%d]\n", bestDistance, numChanges);
		}
		
		if((iterations % 100) == 0)
		{
		}

		if (GetKeyState(' ') & 0x8000)
		{
			break;
		}
		
		iterations--;
	}*/
	
	sort(bestColourIndices.begin(), bestColourIndices.end());
	GenerateMixColours(mixColours, paletteColours, bestColourIndices);
	
	vector<uint8_t> outputPaletteColours;
	
	for(int n = 0; n < bestColourIndices.size(); n++)
	{
		for(int c = 0; c < 4; c++)
		{
			outputPaletteColours.push_back(paletteColours[bestColourIndices[n] * 4 + c]);
		}
	}
	
	LinearToGamma(outputPaletteColours);
	LinearToGamma(mixColours);
	lodepng::encode("outpal.png", outputPaletteColours, bestColourIndices.size(), 1);
	lodepng::encode("blendpal.png", mixColours, bestColourIndices.size(), bestColourIndices.size());

	vector<uint8_t> matchColours;
	int numMixColours = mixColours.size() / 4;
	for(int n = 0; n < numPaletteColours; n++)
	{
		int bestMixIndex = -1;
		float bestMixDistance = 0;
		
		for(int c = 0; c < numMixColours; c++)
		{
			//float distance = 0;
			//for(int i = 0; i < 3; i++)
			//{
			//	distance += (mixColours[c * 4 + i] - targetPaletteColours[n * 4 + i]) * (mixColours[c * 4 + i] - targetPaletteColours[n * 4 + i]);
			//}
			float distance = CalcDistance(mixColours[c * 4], mixColours[c * 4 + 1], mixColours[c * 4 + 2], targetPaletteColours[n * 4], targetPaletteColours[n * 4 + 1], targetPaletteColours[n * 4 + 2]);

			if(bestMixIndex == -1 || distance < bestMixDistance)
			{
				bestMixDistance = distance;
				bestMixIndex = c;
			}
		}
		
		for(int i = 0; i < 4; i++)
		{
			matchColours.push_back(mixColours[bestMixIndex * 4 + i]);
		}
	}
	LinearToGamma(matchColours);
	lodepng::encode("matchpal.png", matchColours, width, height);
	
	return 0;
}

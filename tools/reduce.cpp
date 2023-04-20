#include <stdint.h>
#include <stdio.h>
#include <vector>
#include "lodepng.cpp"

//#define MATCH_PAIR_THRESHOLD 120
//#define MATCH_PAIR_THRESHOLD 200
//#define MATCH_PAIR_THRESHOLD 80
#define MATCH_PAIR_THRESHOLD 120
bool ditherOutput = true;

int matchPairThreshold = MATCH_PAIR_THRESHOLD;

using namespace std;

uint8_t GammaToLinear(uint8_t x)
{
	return (uint8_t)(255.0 * pow((x / 255.0), 2.2));
}

uint8_t LinearToGamma(uint8_t x)
{
	return (uint8_t)(255.0 * pow((x / 255.0), 1.0 / 2.2));
}

void GenerateMixColours(vector<uint8_t>& mixColours, vector<uint8_t>& paletteColours)
{
	mixColours.clear();
	
	int numPaletteColours = paletteColours.size() / 4;

	for(int x = 0; x < numPaletteColours; x++)
	{
		for(int y = 0; y < numPaletteColours; y++)
		{
			int first = y;
			int second = x;
			
			mixColours.push_back(LinearToGamma((uint8_t)((GammaToLinear(paletteColours[first * 4 + 0]) + GammaToLinear(paletteColours[second * 4 + 0])) / 2)));
			mixColours.push_back(LinearToGamma((uint8_t)((GammaToLinear(paletteColours[first * 4 + 1]) + GammaToLinear(paletteColours[second * 4 + 1])) / 2)));
			mixColours.push_back(LinearToGamma((uint8_t)((GammaToLinear(paletteColours[first * 4 + 2]) + GammaToLinear(paletteColours[second * 4 + 2])) / 2)));
			mixColours.push_back(255);
		}
	}
}

float CalcDistance(int r1, int g1, int b1, int r2, int g2, int b2)
{
	int rmean = (r1 + r2) / 2;
	int r = r1 - r2;
	int g = g1 - g2;
	int b = b1 - b2;
	return (float)(sqrt((((512+rmean)*r*r)>>8) + 4*g*g + (((767-rmean)*b*b)>>8)));
}


int main(int argc, char* argv[])
{
	if(argc < 2)
	{
		printf("Usage: %s [image.png]\n", argv[0]);
		return 1;
	}
	if(argc > 2)
	{
		matchPairThreshold = atoi(argv[2]);
		if(!matchPairThreshold)
		{
			matchPairThreshold = MATCH_PAIR_THRESHOLD;
		}
	}

	vector<uint8_t> paletteColours;
	unsigned width, height;
	
	if(lodepng::decode(paletteColours, width, height, "outpal.png"))
	{
		printf("Error opening palette file\n");
		return 0;
	}
	
	int numPaletteColours = width * height;
	
	vector<uint8_t> inputImage;
	
	if(lodepng::decode(inputImage, width, height, argv[1]))
	{
		printf("Error opening %s\n", argv[1]);
		return 0;
	}

	vector<uint8_t> mixColours;
	GenerateMixColours(mixColours, paletteColours);

	lodepng::encode("blendpal.png", mixColours, numPaletteColours, numPaletteColours);

	vector<uint8_t> outputImage;
	int numMixColours = numPaletteColours * numPaletteColours;
	
	const int numLightingVariations = 1;
	
	for(int z = 0; z < numLightingVariations; z++)
	{
		float lightingScale = ((float)(numLightingVariations - z)) / numLightingVariations;
		
		for(unsigned y = 0; y < height; y++)
		{
			for(unsigned x = 0; x < width; x++)
			{
				int bestMixDistance = 0;
				int bestMixIndex = -1;
				int imageIndex = y * width + x;

				if(inputImage[imageIndex * 4 + 3] < 255 || (inputImage[imageIndex * 4] == 0 && inputImage[imageIndex * 4 + 1] == 255 && inputImage[imageIndex * 4 + 2] == 255))
				{
					// Transparent
					outputImage.push_back(0);
					outputImage.push_back(255);
					outputImage.push_back(255);
					outputImage.push_back(0);
					continue;
				}
				
				for(int c = 0; c < numMixColours; c++)
				{
					//float distance = 0;
					//for(int i = 0; i < 3; i++)
					//{
					//	uint8_t channelValue = (uint8_t)(lightingScale * inputImage[imageIndex * 4 + i]);
					//	distance += (mixColours[c * 4 + i] - channelValue) * (mixColours[c * 4 + i] - channelValue);
					//}
					float distance = CalcDistance(mixColours[c * 4], mixColours[c * 4 + 1], mixColours[c * 4 + 2], lightingScale * inputImage[imageIndex * 4], lightingScale * inputImage[imageIndex * 4 + 1], lightingScale * inputImage[imageIndex * 4 + 2]);
					
					int pairDistance = 0;
					int pairFirst = c / numPaletteColours;
					int pairSecond = c % numPaletteColours;
					for(int i = 0; i < 3; i++)
					{
						int dist = paletteColours[pairFirst * 4 + i] - paletteColours[pairSecond * 4 + i];
						pairDistance += dist * dist;
					}
					if(pairDistance > matchPairThreshold * matchPairThreshold)
					{
						continue;
					}			
					
					if(bestMixIndex == -1 || distance < bestMixDistance)
					{
						bestMixDistance = distance;
						bestMixIndex = c;
					}
				}
			
				int first = bestMixIndex / numPaletteColours;
				int second = bestMixIndex % numPaletteColours;
				
				if(x & 1)
				{
					int temp = first;
					first = second;
					second = temp;
				}
				
				if((y & 1) && ditherOutput)
				{
					for(int i = 0; i < 4; i++)
					{
						outputImage.push_back(paletteColours[second * 4 + i]);
					}
					//for(int i = 0; i < 4; i++)
					//{
					//	outputImage.push_back(paletteColours[first * 4 + i]);
					//}
				}
				else
				{
					for(int i = 0; i < 4; i++)
					{
						outputImage.push_back(paletteColours[first * 4 + i]);
					}
					//for(int i = 0; i < 4; i++)
					//{
					//	outputImage.push_back(paletteColours[second * 4 + i]);
					//}
				}
			}
		}
	}
	
	lodepng::encode("reduced.png", outputImage, width, height * numLightingVariations);

	return 0;
}

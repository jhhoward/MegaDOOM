#include <stdint.h>
#include <stdio.h>
#include <vector>
#include "lodepng.cpp"

//#define MATCH_PAIR_THRESHOLD 120
#define MATCH_PAIR_THRESHOLD 200
bool ditherOutput = false;

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

int main(int argc, char* argv[])
{
	if(argc != 2)
	{
		printf("Usage: %s [image.png]\n", argv[0]);
		return 1;
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
	
	const int numLightingVariations = 4;
	
	for(int z = 0; z < numLightingVariations; z++)
	{
		float lightingScale = ((float)(numLightingVariations - z)) / numLightingVariations;
		
		for(unsigned y = 0; y < height; y++)
		{
			for(unsigned x = 0; x < width; x += 2)
			{
				int bestMixDistance = 0;
				int bestMixIndex = -1;
				int imageIndex = y * width + x;
				
				for(int c = 0; c < numMixColours; c++)
				{
					float distance = 0;
					for(int i = 0; i < 3; i++)
					{
						uint8_t channelValue = (uint8_t)(lightingScale * inputImage[imageIndex * 4 + i]);
						distance += (mixColours[c * 4 + i] - channelValue) * (mixColours[c * 4 + i] - channelValue);
					}
					
					int pairDistance = 0;
					int pairFirst = c / numPaletteColours;
					int pairSecond = c % numPaletteColours;
					for(int i = 0; i < 3; i++)
					{
						int dist = paletteColours[pairFirst * 4 + i] - paletteColours[pairSecond * 4 + i];
						pairDistance += dist * dist;
					}
					if(pairDistance > MATCH_PAIR_THRESHOLD * MATCH_PAIR_THRESHOLD)
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
				
				if((y & 1) && ditherOutput)
				{
					for(int i = 0; i < 4; i++)
					{
						outputImage.push_back(paletteColours[second * 4 + i]);
					}
					for(int i = 0; i < 4; i++)
					{
						outputImage.push_back(paletteColours[first * 4 + i]);
					}
				}
				else
				{
					for(int i = 0; i < 4; i++)
					{
						outputImage.push_back(paletteColours[first * 4 + i]);
					}
					for(int i = 0; i < 4; i++)
					{
						outputImage.push_back(paletteColours[second * 4 + i]);
					}
				}
			}
		}
	}
	
	lodepng::encode("reduced.png", outputImage, width, height * numLightingVariations);

	return 0;
}

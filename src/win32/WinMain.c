#include <stdint.h>
#include <stdbool.h>
#include <SDL.h>
#include <stdio.h>
#include "lodepng.h"
#include "../tables.h"
#include "../r_local.h"

// For MD:
// - Preprocess WAD and extract data
// - Dump to big header / .c file as const structs / arrays of structs
// - Automatically fixes up little endian / big endianess
// - Loading a map just sets the pointers to the static map data and clearing / initing the dynamic map data

// For x86
// - Load vanilla WAD? 

#define ZOOM_SCALE 1
//#define DISPLAY_WIDTH 256
//#define DISPLAY_HEIGHT 224
#define DISPLAY_WIDTH 640
#define DISPLAY_HEIGHT 480


#define INPUT_LEFT 1
#define INPUT_RIGHT 2
#define INPUT_UP 4
#define INPUT_DOWN 8
#define INPUT_A 16
#define INPUT_B 32

#define TARGET_FRAMERATE 30


typedef struct
{
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Surface* screenSurface;
	SDL_Texture* screenTexture;
	int width, height;
	int zoom;
} display_window_t;

display_window_t mainWindow;
display_window_t debugMapWindow;

uint8_t InputMask = 0;

typedef struct 
{
	SDL_Scancode key;
	uint8_t mask;
} KeyMap;

#define NUM_KEY_MAPPINGS sizeof(KeyMappings) / sizeof(KeyMap)

KeyMap KeyMappings[] =
{
	{ SDL_SCANCODE_LEFT, INPUT_LEFT },
	{ SDL_SCANCODE_RIGHT, INPUT_RIGHT },
	{ SDL_SCANCODE_UP, INPUT_UP },
	{ SDL_SCANCODE_DOWN, INPUT_DOWN },
	{ SDL_SCANCODE_Z, INPUT_A },
	{ SDL_SCANCODE_X, INPUT_B },
};

void PutPixelImmediate(SDL_Surface* surface, int x, int y, uint32_t col)
{
	if (x < 0 || y < 0 || x >= surface->w || y >= surface->h)
	{
		return;
	}

	int bpp = surface->format->BytesPerPixel;
	Uint8* p = (Uint8*)surface->pixels + y * surface->pitch + x * bpp;

	*(Uint32*)p = col;
}


uint8_t GetInput()
{
	uint8_t inputMask = 0;

	const uint8_t* keyStates = SDL_GetKeyboardState(NULL);

	for (unsigned int n = 0; n < NUM_KEY_MAPPINGS; n++)
	{
		if (keyStates[KeyMappings[n].key])
		{
			inputMask |= KeyMappings[n].mask;
		}
	}

	return inputMask;
}

void LoadWad(const char* filename);

void RenderAll();

void DrawMapDebugLine(int x0, int y0, int x1, int y1)
{
	int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
	int dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
	int err = dx + dy, e2; /* error value e_xy */

	for (;;) {  /* loop */
		PutPixelImmediate(debugMapWindow.screenSurface, x0, y0, 0xffffffff);
		if (x0 == x1 && y0 == y1) break;
		e2 = 2 * err;
		if (e2 >= dy) { err += dy; x0 += sx; } /* e_xy+e_x > 0 */
		if (e2 <= dx) { err += dx; y0 += sy; } /* e_xy+e_y < 0 */
	}
}

void DrawDebugLine(int x0, int y0, int x1, int y1)
{
	int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
	int dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
	int err = dx + dy, e2; /* error value e_xy */

	for (;;) {  /* loop */
		PutPixelImmediate(mainWindow.screenSurface, x0, y0, 0xffffffff);
		if (x0 == x1 && y0 == y1) break;
		e2 = 2 * err;
		if (e2 >= dy) { err += dy; x0 += sx; } /* e_xy+e_x > 0 */
		if (e2 <= dx) { err += dx; y0 += sy; } /* e_xy+e_y < 0 */
	}
}

extern int16_t viewx, viewy;


void OpenDisplayWindow(display_window_t* window, int width, int height, int zoom)
{
	window->width = width;
	window->height = height;
	window->zoom = zoom;

	SDL_CreateWindowAndRenderer(width * zoom, height * zoom, SDL_WINDOW_RESIZABLE, &window->window, &window->renderer);
	SDL_RenderSetLogicalSize(window->renderer, width, height);

	window->screenSurface = SDL_CreateRGBSurface(0, width, height, 32,
		0x000000ff,
		0x0000ff00,
		0x00ff0000,
		0xff000000
	);
	window->screenTexture = SDL_CreateTexture(window->renderer, SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_STREAMING, window->screenSurface->w, window->screenSurface->h);
}

void ClearDisplayWindow(display_window_t* window)
{
	SDL_SetRenderDrawColor(window->renderer, 206, 221, 231, 255);
	SDL_RenderClear(window->renderer);
	memset(window->screenSurface->pixels, 0, window->screenSurface->w * window->screenSurface->h * sizeof(uint32_t));
}

void BlitDisplayWindow(display_window_t* window)
{
	SDL_UpdateTexture(window->screenTexture, NULL, window->screenSurface->pixels, window->screenSurface->pitch);
	SDL_Rect src, dest;
	src.x = src.y = dest.x = dest.y = 0;
	src.w = window->width;
	src.h = window->height;
	dest.w = window->width;
	dest.h = window->height;
	SDL_RenderCopy(window->renderer, window->screenTexture, &src, &dest);
	SDL_RenderPresent(window->renderer);

}

void VLine(int x, int y, int count, uint32_t colour)
{
	while (count--)
	{
		bool overdraw = false;

		{
			if (x < 0 || y < 0 || x >= mainWindow.screenSurface->w || y >= mainWindow.screenSurface->h)
			{
				return;
			}

			int bpp = mainWindow.screenSurface->format->BytesPerPixel;
			Uint8* p = (Uint8*)mainWindow.screenSurface->pixels + y * mainWindow.screenSurface->pitch + x * bpp;

			if (*(Uint32*)p)
			{
				overdraw = true;
			}
		}

		if (overdraw && ((y ^ x) & 1))
		{
			PutPixelImmediate(mainWindow.screenSurface, x, y, 0xffff00ff);
		}
		else
		{
			PutPixelImmediate(mainWindow.screenSurface, x, y, colour | 0xff000000);
		}
		y++;
	}
}

void RenderDebugMap(void)
{
#if 1
	int centerX = debugMapWindow.width / 2;
	int centerY = debugMapWindow.height / 2;
	int range = debugMapWindow.width / 2;;
	DrawMapDebugLine(centerX, centerY, centerX + range, centerY - range);
	DrawMapDebugLine(centerX, centerY, centerX - range, centerY - range);
#else
	angle_t left = viewangle - ANG45 + ANG90;
	angle_t right = viewangle + ANG45 + ANG90;

	int leftY = finecosine[left >> ANGLETOFINESHIFT];
	int leftX = finesine[left >> ANGLETOFINESHIFT];

	int rightY = finecosine[right >> ANGLETOFINESHIFT];
	int rightX = finesine[right >> ANGLETOFINESHIFT];

	int centerX = debugMapWindow.width / 2;
	int centerY = debugMapWindow.height / 2;
	DrawMapDebugLine(centerX, centerY, centerX + leftX, centerY + leftY);
	DrawMapDebugLine(centerX, centerY, centerX + rightX, centerY + rightY);
#endif
}

int main(int argc, char* argv[])
{
	SDL_Init(SDL_INIT_EVERYTHING);

	OpenDisplayWindow(&debugMapWindow, 640, 400, 1);
	OpenDisplayWindow(&mainWindow, 256, 224, 2);

	{
		int windowX, windowY;
		SDL_GetWindowPosition(debugMapWindow.window, &windowX, &windowY);
		SDL_SetWindowPosition(mainWindow.window, windowX - debugMapWindow.width / 2, windowY);
		SDL_SetWindowPosition(debugMapWindow.window, windowX + debugMapWindow.width / 2, windowY);
	}

	R_InitTables();
	R_ExecuteSetViewSize();

	for (int n = 0; n < 360; n++)
	{
		float radians = n * 3.141592654 / 180;
		int x = (int)(1024 * cos(radians));
		int y = (int)(1024 * sin(radians));

		angle_t bam = R_PointToAngle(x, y);
		int degrees = bam * 360 / ANG_MAX;
		printf("%d : %d\n", n, degrees);
	}

	//LoadWad("test.wad");
	LoadWad("doom1.wad");

	//SDL_SetWindowPosition(AppWindow, 1900 - DISPLAY_WIDTH * 2, 1020 - DISPLAY_HEIGHT);

	/*
	SDL_AudioSpec wanted;
	wanted.freq = audioSampleRate;
	wanted.format = AUDIO_U8;
	wanted.channels = 1;
	wanted.samples = 4096;
	wanted.callback = FillAudioBuffer;

	if (SDL_OpenAudio(&wanted, NULL) < 0) {
		printf("Error: %s\n", SDL_GetError());
	}
	SDL_PauseAudio(0);
	*/
	//DumpFont();

	bool running = true;
	int playRate = 1;
	static int testAudio = 0;

	while (running)
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT:
				running = false;
				break;
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym)
				{
				case SDLK_ESCAPE:
					running = false;
					break;
				case SDLK_F12:
				{
					lodepng_encode_file("screenshot.png", (unsigned char*)(mainWindow.screenSurface->pixels), mainWindow.screenSurface->w, mainWindow.screenSurface->h, LCT_RGBA, 8);
				}
				break;
				}
				break;
			case SDL_KEYUP:
				break;
			}
		}


		uint8_t input = GetInput();
		int scrollSpeed = 10;
		if (input & INPUT_LEFT)
		{
			//viewx-= scrollSpeed;
			viewangle += ANG1 * 5;
		}
		if (input & INPUT_RIGHT)
		{
			//viewx+= scrollSpeed;
			viewangle -= ANG1 * 5;
		}
		if (input & INPUT_UP)
		{
			viewx += finecosine[viewangle >> ANGLETOFINESHIFT] >> 5;
			viewy += finesine[viewangle >> ANGLETOFINESHIFT] >> 5;
		}
		if (input & INPUT_DOWN)
		{
			viewx -= finecosine[viewangle >> ANGLETOFINESHIFT] >> 5;
			viewy -= finesine[viewangle >> ANGLETOFINESHIFT] >> 5;
		}
		if (input & INPUT_A)
		{
			viewz += 3;
		}
		if (input & INPUT_B)
		{
			viewz -= 3;
		}

		ClearDisplayWindow(&debugMapWindow);
		ClearDisplayWindow(&mainWindow);

		// Draw stuff here
		//RenderBSPNode(0);
		RenderAll();
		RenderDebugMap();

		//DrawDebugLine(10, 10, 50, 23);

		BlitDisplayWindow(&debugMapWindow);
		BlitDisplayWindow(&mainWindow);

		SDL_Delay(1000 / TARGET_FRAMERATE);
	}

	return 0;
}

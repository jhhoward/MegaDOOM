#include "genesis.h"

uint16_t rand(void);
void srand(uint16_t seed);

#include "../../src/generated/palette.inc"
#include "../../src/r_bsp.c"
#include "../../src/tables.c"
#include "../../src/r_main.c"
#include "../../src/Render.c"
#include "../../project/E1M1.inc.h"


#define FRAMEBUFFER_WIDTH 128
#define FRAMEBUFFER_HEIGHT 128
#define FRAMEBUFFER_TILE_BYTES (4 * 8)
#define FRAMEBUFFER_WIDTH_TILES (FRAMEBUFFER_WIDTH / 4)
#define FRAMEBUFFER_HEIGHT_TILES (FRAMEBUFFER_HEIGHT / 8)

u8 framebuffer[FRAMEBUFFER_WIDTH * FRAMEBUFFER_HEIGHT];
u16 framebufferTiles[FRAMEBUFFER_WIDTH_TILES * FRAMEBUFFER_HEIGHT_TILES];

const TileSet framebufferTileSet =
{
    COMPRESSION_NONE, FRAMEBUFFER_WIDTH_TILES * FRAMEBUFFER_HEIGHT_TILES, (u32*)framebuffer
};

const TileMap framebufferTileMap =
{
    COMPRESSION_NONE, FRAMEBUFFER_WIDTH_TILES, FRAMEBUFFER_HEIGHT_TILES, framebufferTiles
};

void DrawMapDebugLine(int x0, int y0, int x1, int y1, uint32_t colour)
{

}

static uint16_t xs = 1;

uint16_t rand()
{
    xs ^= xs << 7;
    xs ^= xs >> 9;
    xs ^= xs << 8;
    return xs;
}

void srand(uint16_t seed)
{
    xs = seed | 1;
}


void putpixel(int x, int y, u8 colour)
{
    u8* ptr = framebuffer;
    ptr += (x >> 2)* (FRAMEBUFFER_HEIGHT_TILES * FRAMEBUFFER_TILE_BYTES);
    ptr += (x & 3);
    ptr += (y << 2);
    *ptr = colour;
}

void VLine(int x, int y, int count, uint8_t colour)
{
    u8* ptr = framebuffer;
    ptr += (x >> 2) * (FRAMEBUFFER_HEIGHT_TILES * FRAMEBUFFER_TILE_BYTES);
    ptr += (x & 3);
    ptr += (y << 2);

    while (count--)
    {
        if (y >= FRAMEBUFFER_HEIGHT)
        //if (x < 0 || y < 0 || x >= FRAMEBUFFER_WIDTH || y >= FRAMEBUFFER_HEIGHT)
        {
            return;
        }

        *ptr = colour;
        ptr += 4;
        y++;
    }
}


int main(bool hardReset)
{ 
    // disable interrupt when accessing VDP
    SYS_disableInts();

    VDP_setScreenWidth256();
//    VDP_drawText("Hello world !", 12, 12);

    PAL_setColors(0, gamePalette, 16, CPU);

    for (int y = 0; y < FRAMEBUFFER_HEIGHT_TILES; y++)
    {
        for (int x = 0; x < FRAMEBUFFER_WIDTH_TILES; x++)
        {
            framebufferTiles[y * FRAMEBUFFER_WIDTH_TILES + x] = TILE_USER_INDEX + x * FRAMEBUFFER_HEIGHT_TILES + y;
        }
    }
    for (int y = 0; y < FRAMEBUFFER_HEIGHT; y++)
    {
        for (int x = 0; x < FRAMEBUFFER_WIDTH; x++)
        {
            //putpixel(x, y, (u8)x);
        }
    }
    for (int n = 0; n < FRAMEBUFFER_WIDTH * FRAMEBUFFER_HEIGHT; n++)
    {
        u8 z = (n >> 5) & 0xf;

        framebuffer[n] = z | (z << 4);
    }

    VDP_setTileMap(BG_A, &framebufferTileMap, 0, 0, FRAMEBUFFER_WIDTH_TILES, FRAMEBUFFER_HEIGHT_TILES, CPU);

    for (int n = 0; n < 64; n++)
    {
        putpixel(n, n, 0xdd);
    }
    //    
    VDP_loadTileSet(&framebufferTileSet, TILE_USER_INDEX, CPU);
    //VDP_setTileMapData(VDP_BG_A, (u16*) framebuffer, TILE_USER_INDEX, FRAMEBUFFER_HEIGHT_TILES * FRAMEBUFFER_WIDTH_TILES, 2, CPU);

    // VDP process done, we can re enable interrupts
    SYS_enableInts();

    int i = 0;
    u8 col = 0;

    currentlevel = &map_E1M1;
    viewx = currentlevel->things[0].x;
    viewy = currentlevel->things[0].y;

    while(TRUE)
    {
        u16 input = JOY_readJoypad(JOY_1);

        if (input & BUTTON_LEFT)
        {
            //viewx-= scrollSpeed;
            viewangle += ANG1 * 5;
        }
        if (input & BUTTON_RIGHT)
        {
            //viewx+= scrollSpeed;
            viewangle -= ANG1 * 5;
        }
        if (input & BUTTON_UP)
        {
            viewx += finecosine[viewangle >> ANGLETOFINESHIFT] >> 5;
            viewy += finesine[viewangle >> ANGLETOFINESHIFT] >> 5;
        }
        if (input & BUTTON_DOWN)
        {
            viewx -= finecosine[viewangle >> ANGLETOFINESHIFT] >> 5;
            viewy -= finesine[viewangle >> ANGLETOFINESHIFT] >> 5;
        }
        if (input & BUTTON_A)
        {
            viewz += 3;
        }
        if (input & BUTTON_B)
        {
            viewz -= 3;
        }

        RenderAll();

        VDP_loadTileSet(&framebufferTileSet, TILE_USER_INDEX, DMA_QUEUE);

        // always call this method at the end of the frame
        SYS_doVBlankProcess();
    }

    return 0;
}

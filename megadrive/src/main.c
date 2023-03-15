#include "genesis.h"

#include "../../src/palette.inc"
#include "../../src/r_bsp.c"
#include "../../src/tables.c"
#include "../../src/r_main.c"


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

void putpixel(int x, int y, u8 colour)
{
    u8* ptr = framebuffer;
    ptr += (x >> 2)* (FRAMEBUFFER_HEIGHT_TILES * FRAMEBUFFER_TILE_BYTES);
    ptr += (x & 3);
    ptr += (y << 2);
    *ptr = colour;
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

    while(TRUE)
    {
        i++;
        if (i > FRAMEBUFFER_WIDTH)
        {
            i = 0;
            col++;
        }
        putpixel(i, 10, col);
        putpixel(i, 11, col);
        putpixel(i, 12, col);
        putpixel(i, 13, col);
        putpixel(i, 14, col);
        putpixel(i, 15, col);

        VDP_loadTileSet(&framebufferTileSet, TILE_USER_INDEX, DMA_QUEUE);

        // always call this method at the end of the frame
        SYS_doVBlankProcess();
    }

    return 0;
}

#include "genesis.h"

uint16_t rand(void);
void srand(uint16_t seed);

#include "../../src/generated/palette.inc"
#include "../../src/r_bsp.c"
#include "../../src/tables.c"
#include "../../src/r_main.c"
#include "../../src/Render.c"
#include "../../project/E1M1.inc.h"
//#include "../../project/E1M2.inc.h"


#define FRAMEBUFFER_WIDTH VIEWPORT_WIDTH
#define FRAMEBUFFER_HEIGHT VIEWPORT_HEIGHT
#define FRAMEBUFFER_TILE_BYTES (4 * 8)
#define FRAMEBUFFER_WIDTH_TILES (FRAMEBUFFER_WIDTH / 4)
#define FRAMEBUFFER_HEIGHT_TILES (FRAMEBUFFER_HEIGHT / 8)
#define FRAMEBUFFER_X ((32 - FRAMEBUFFER_WIDTH_TILES) / 2)
#define FRAMEBUFFER_Y ((28 - 4 - FRAMEBUFFER_HEIGHT_TILES) / 2)

u8 framebuffer[FRAMEBUFFER_WIDTH * FRAMEBUFFER_HEIGHT];
u16 framebufferTiles[FRAMEBUFFER_WIDTH_TILES * FRAMEBUFFER_HEIGHT_TILES];

#include "../../src/generated/framebuffer.inc.h"

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

void TexturedLine(const walltexture_t* texture, int16_t x, int16_t y, int16_t count, int16_t u, int16_t v, int16_t step)
{
    u &= (texture->width - 1);

    u8* ptr = framebuffer + framebufferx[x];
    const u8* texptr = texture->columns[u];
    ptr += (y << 2);


    int texcoord = v << 8;

    while(count--)
    {
        texcoord += step;
        *ptr = texptr[(texcoord >> 8) & 127];
        ptr += 4;
    }
}


void VLineRef(int x, int y, int count, uint8_t colour)
{
    u8* ptr = framebuffer + framebufferx[x];
    //ptr += (x >> 2) * (FRAMEBUFFER_HEIGHT_TILES * FRAMEBUFFER_TILE_BYTES);
    //ptr += (x & 3);
    ptr += (y << 2);

    while (count--)
    {
        //if (y >= FRAMEBUFFER_HEIGHT)
        //if (x < 0 || y < 0 || x >= FRAMEBUFFER_WIDTH || y >= FRAMEBUFFER_HEIGHT)
        //{
        //    return;
        //}

        *ptr = colour;
        ptr += 4;
        //y++;
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
//    VDP_setTileMap(BG_A, &framebufferTileMap, FRAMEBUFFER_X, FRAMEBUFFER_Y, FRAMEBUFFER_WIDTH_TILES, FRAMEBUFFER_HEIGHT_TILES, CPU);

    for (int n = 0; n < 64; n++)
    {
        putpixel(n, n, 0xdd);
    }
    //    
    VDP_loadTileSet(&framebufferTileSet, TILE_USER_INDEX, CPU);
    //VDP_setTileMapData(VDP_BG_A, (u16*) framebuffer, TILE_USER_INDEX, FRAMEBUFFER_HEIGHT_TILES * FRAMEBUFFER_WIDTH_TILES, 2, CPU);

    //VDP_setVerticalScroll(BG_A, -(224 - 32 - VIEWPORT_HEIGHT) / 2);
    //VDP_setHorizontalScroll(BG_A, -(128 - VIEWPORT_WIDTH) / 2);

    // VDP process done, we can re enable interrupts
    SYS_enableInts();

    int i = 0;
    u8 col = 0;

    currentlevel = &map_E1M1;
    viewx = currentlevel->things[0].x;
    viewy = currentlevel->things[0].y;

    u32 lasttick = getTick();

    while(TRUE)
    {
        u16 input = JOY_readJoypad(JOY_1);

        int elapsed = getTick() - lasttick;
        int frames = elapsed >> 4;
        if (frames > 10)
            frames = 10;

        for (int n = 0; n < frames; n++)
        {

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
        }
        lasttick = getTick();

        R_RenderView();


//        VDP_loadTileSet(&framebufferTileSet, TILE_USER_INDEX, DMA_QUEUE);
        VDP_loadTileSet(&framebufferTileSet, TILE_USER_INDEX, DMA);

        // always call this method at the end of the frame
        //SYS_doVBlankProcess();
        SYS_doVBlankProcessEx(IMMEDIATELY);
    }

    return 0;
}

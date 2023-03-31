#include "genesis.h"

uint16_t rand(void);
void srand(uint16_t seed);

#include "../../src/generated/palette.inc.h"
#include "../../src/r_bsp.c"
#include "../../src/tables.c"
#include "../../src/r_main.c"
#include "../../src/Render.c"
//#include "../../src/generated/E1M1.inc.h"
//#include "../../src/generated/E1M2.inc.h"
//#include "../../src/generated/E1M3.inc.h"
#include "../../src/generated/E1M4.inc.h"
//#include "../../src/generated/E1M5.inc.h"
//#include "../../src/generated/E1M6.inc.h"
//#include "../../src/generated/E1M7.inc.h"
//#include "../../src/generated/E1M8.inc.h"
//#include "../../src/generated/E1M9.inc.h"
#include "music.h"

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

//const map_t* maps[] =
//{
//    &map_E1M1,
//    &map_E1M2,
//    &map_E1M3,
//    &map_E1M4,
//    &map_E1M5,
//    &map_E1M6,
//    &map_E1M7,
//    &map_E1M8,
//    &map_E1M9
//};
//
//const u8 const* musictracks[] =
//{
//    xgm_e1m1,
//    xgm_e1m2,
//    xgm_e1m3,
//    xgm_e1m4,
//    xgm_e1m5,
//    xgm_e1m6,
//    xgm_e1m7,
//    xgm_e1m8,
//    xgm_e1m9
//};

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

void TexturedLineRef(const walltexture_t* texture, int16_t x, int16_t y, int16_t count, int16_t u, int16_t v, int16_t step)
{
  //  u &= (texture->width - 1);

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

int currentlevelnum = 0;

void SetLevel(int levelnum)
{
//    currentlevelnum = levelnum;
//    currentlevel = maps[levelnum];
//    viewx = currentlevel->things[0].x;
//    viewy = currentlevel->things[0].y;
//    viewangle = currentlevel->things[0].angle;
//
//    XGM_startPlay(musictracks[levelnum]);
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

//    SetLevel(0);
    currentlevel = &map_E1M4;
    viewx = currentlevel->things[0].x;
    viewy = currentlevel->things[0].y;
    viewangle = currentlevel->things[0].angle;

    XGM_startPlay(xgm_e1m4);


    u32 lasttick = getTick();
    u32 fpstimer = getTime(0) + 256;
    int framecount = 0;
    bool changelevel = false;

    char fpsstring[10] = "FPS:     ";

    while(TRUE)
    {
        u16 input = JOY_readJoypad(JOY_1);

        if (getTime(0) > fpstimer)
        {
            int i = 0;
            while (framecount > 0)
            {
                int part = framecount % 10;
                fpsstring[9 - i] = '0' + part;
                i--;
                framecount /= 10;
            }
            VDP_drawText(fpsstring, 0, 24);

            framecount = 0;
            fpstimer = getTime(0) + 256;
        }
        framecount++;

        int elapsed = getTick() - lasttick;
        int frames = elapsed >> 4;
        if (frames > 10)
            frames = 10;

        //viewangle += ANG1;
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
            if (input & BUTTON_START)
            {
                if (!changelevel)
                {
                    changelevel = true;
                }
            }
            else
            {
                if (changelevel)
                {
                    changelevel = false;
                    currentlevelnum++;
                    if (currentlevelnum == 9)
                    {
                        currentlevelnum = 0;
                    }
                    SetLevel(currentlevelnum);
                }
            }
        }
        lasttick = getTick();

        R_RenderView();


//        VDP_loadTileSet(&framebufferTileSet, TILE_USER_INDEX, DMA_QUEUE);
        VDP_loadTileSet(&framebufferTileSet, TILE_USER_INDEX, DMA);

        // always call this method at the end of the frame
        SYS_doVBlankProcess();
        //SYS_doVBlankProcessEx(IMMEDIATELY);
    }

    return 0;
}

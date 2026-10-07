#include "common.h"

typedef signed char s8;
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;

typedef struct { u32 w0; u32 w1; } Gfx;

#define GFX_WORDS(pkt, c0, c1)  \
    {                           \
        Gfx *_g = (pkt)++;      \
        _g->w0 = (c0);          \
        _g->w1 = (c1);          \
    }

Gfx *func_8007CA78(Gfx *gfx) {
    GFX_WORDS(gfx, 0xE7000000, 0);
    GFX_WORDS(gfx, 0xE3000A01, 0);
    GFX_WORDS(gfx, 0xD7000002, 0x80008000);
    GFX_WORDS(gfx, 0xD9000000, 0);
    GFX_WORDS(gfx, 0xFA000000, 0xFFFFFFFF);
    GFX_WORDS(gfx, 0xFB000000, 0xFF);
    GFX_WORDS(gfx, 0xF8000000, 0xFFFFFFFF);
    GFX_WORDS(gfx, 0xE3000F00, 0);
    GFX_WORDS(gfx, 0xE3000D01, 0);
    GFX_WORDS(gfx, 0xE3000C00, 0);
    GFX_WORDS(gfx, 0xE3001201, 0);
    GFX_WORDS(gfx, 0xE3001402, 0xC00);
    GFX_WORDS(gfx, 0xE3001700, 0);
    GFX_WORDS(gfx, 0xE2001E01, 0);
    GFX_WORDS(gfx, 0xE3001801, 0xC0);
    GFX_WORDS(gfx, 0xE200001C, 0x504240);
    GFX_WORDS(gfx, 0xFC309A61, 0x5536FF7F);
    return gfx;
}

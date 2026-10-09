#include "common.h"

typedef unsigned char u8;
typedef struct { u32 w0; u32 w1; } Gfx;
extern const Gfx D_8013B6E8[];
/* Original 80060244/80060344..378 retains all 32 width bits and converts
 * them unsigned; the registry's u8 width and integer texture are false. */
extern Gfx *func_80060230(Gfx *gfx, const void *image, s32 format, u32 width, u32 height, s32 x, s32 y, float scaleX, float scaleY);

/* ODD_C: do { } while (0) multi-statement macro idiom (as libultra gbi.h _DW), so each use is one
 * statement before its break. No scheduling effect: the brace-only form also matches (0 words). */
#define PALETTE_COMMANDS(gfx, palette, entries) do { \
    { Gfx *packet = (gfx)++; \
      packet->w0 = 0xFD100000; packet->w1 = (u32)(palette); } \
    { Gfx *packet = (gfx)++; \
      packet->w0 = 0xE8000000; packet->w1 = 0; } \
    { Gfx *packet = (gfx)++; \
      packet->w0 = 0xF5000100; packet->w1 = 0x07000000; } \
    { Gfx *packet = (gfx)++; \
      packet->w0 = 0xE6000000; packet->w1 = 0; } \
    { Gfx *packet = (gfx)++; \
      packet->w0 = 0xF0000000; \
      packet->w1 = 0x07000000 | (((entries) - 1) << 14); } \
    { Gfx *packet = (gfx)++; \
      packet->w0 = 0xE7000000; packet->w1 = 0; } \
} while (0)

Gfx *func_80060048(Gfx *gfx, const void *image, const void *palette, s32 format, s32 width, s32 height, s32 x, s32 y, float scaleX, float scaleY, const Gfx *extra)
{
    u8 kind = format & 7;
    u8 size = format & 0x30;
    Gfx *sync = gfx++;
    Gfx *setup = gfx++;
    u8 flags = format;
    sync->w0 = 0xE7000000;
    sync->w1 = 0;
    setup->w0 = 0xDE000000;
    setup->w1 = (u32)D_8013B6E8;
    if (extra != 0) {
        Gfx *packet = gfx++;
        packet->w0 = 0xDE000000;
        packet->w1 = (u32)extra;
    }
    if (format & 4) {
        if (kind == 5) {
            Gfx *packet = gfx++;
            packet->w0 = 0xE3001001;
            packet->w1 = 0x8000;
        } else {
            Gfx *packet = gfx++;
            packet->w0 = 0xE3001001;
            packet->w1 = 0xC000;
        }
        switch (size) {
        case 0:
            PALETTE_COMMANDS(gfx, palette, 16);
            break;
        case 16:
            PALETTE_COMMANDS(gfx, palette, 256);
            break;
        }
    } else {
        Gfx *packet = gfx++;
        packet->w0 = 0xE3001001;
        packet->w1 = 0;
    }
    return func_80060230(gfx, image, flags, width, height, x, y, scaleX, scaleY);
}

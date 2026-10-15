#include "common.h"

typedef struct { u32 w0, w1; } Gfx;
/* Texture subobject of a render effect (func_8006F7F8 passes &object->effect_48):
 * image format/size, clamp/wrap modes, dimensions, scroll offsets and the image. */
typedef struct {
    unsigned char format, size, wrapS, wrapT;
    u32 width, height;
    float offsetS, offsetT;
    char pad14[8];
    void *pixels_1C;
} Texture;
extern s32 func_8006FEA0(u32 value);

/* libultra gbi-style packet helpers. */
#define _SHIFTL(v, s, w) ((u32)(((u32)(v) & ((0x01 << (w)) - 1)) << (s)))
#define gSetWords(pkt, first, second) \
    { Gfx *_g = (pkt); _g->w0 = (first); _g->w1 = (second); }
#define gSetTextureImage(pkt, fmt, siz, width, img) \
    gSetWords(pkt, _SHIFTL(0xFD, 24, 8) | _SHIFTL(fmt, 21, 3) | _SHIFTL(siz, 19, 2) | _SHIFTL((width) - 1, 0, 12), \
              (u32)(img))
#define gSetTile(pkt, fmt, siz, line, tmem, tile, palette, cmt, maskt, shiftt, cms, masks, shifts) \
    gSetWords(pkt, _SHIFTL(0xF5, 24, 8) | _SHIFTL(fmt, 21, 3) | _SHIFTL(siz, 19, 2) | _SHIFTL(line, 9, 9) | \
                   _SHIFTL(tmem, 0, 9), \
              _SHIFTL(tile, 24, 3) | _SHIFTL(palette, 20, 4) | _SHIFTL(cmt, 18, 2) | _SHIFTL(maskt, 14, 4) | \
                   _SHIFTL(shiftt, 10, 4) | _SHIFTL(cms, 8, 2) | _SHIFTL(masks, 4, 4) | _SHIFTL(shifts, 0, 4))
#define gTileGeneric(pkt, c, tile, uls, ult, lrs, lrt) \
    gSetWords(pkt, _SHIFTL(c, 24, 8) | _SHIFTL(uls, 12, 12) | _SHIFTL(ult, 0, 12), \
              _SHIFTL(tile, 24, 3) | _SHIFTL(lrs, 12, 12) | _SHIFTL(lrt, 0, 12))
#define gNoParam(pkt, c) gSetWords(pkt, _SHIFTL(c, 24, 8), 0)

/* Loads the whole texture into TMEM through the load tile, then sets up render
 * tile 0 with its upper-left corner moved by the scroll offset (10.2 fixed point). */
Gfx *func_8006F994(Gfx *cursor, Texture *texture) {
    s32 uls = 0, ult = 0;
    u32 lrs = texture->width - 1;
    u32 lrt = texture->height - 1;
    s32 offsetS = (s32)(texture->offsetS * (float)texture->width * 4.0f);
    s32 offsetT = (s32)(texture->offsetT * (float)texture->height * 4.0f);
    s32 maskS = func_8006FEA0(texture->width);
    s32 maskT = func_8006FEA0(texture->height);
    u32 loadBytes, renderBytes; /* per-texel TILE_BYTES / LINE_BYTES of the size */
    switch (texture->size) {
    case 1: loadBytes = 1; renderBytes = 1; break;
    case 2: loadBytes = 2; renderBytes = 2; break;
    case 3: loadBytes = 2; renderBytes = 2; break;
    default: loadBytes = 0; renderBytes = 0; break;
    }
    gSetWords(cursor++, 0xE3001001, 0);
    if (texture->size == 0) {
        /* 4-bit textures load as 8-bit texels of half the width. */
        gSetTextureImage(cursor++, texture->format, 1, texture->width >> 1, texture->pixels_1C);
        gSetTile(cursor++, texture->format, 1, ((((lrs - uls + 1) >> 1) + 7) >> 3), 0, 7, 0,
                 texture->wrapT, maskT, 0, texture->wrapS, maskS, 0);
        gNoParam(cursor++, 0xE6);
        gTileGeneric(cursor++, 0xF4, 7, uls << 1, ult << 2, lrs << 1, lrt << 2);
        gNoParam(cursor++, 0xE7);
        gSetTile(cursor++, texture->format, 0, ((((lrs - uls + 1) >> 1) + 7) >> 3), 0, 0, 0,
                 texture->wrapT, maskT, 0, texture->wrapS, maskS, 0);
        gTileGeneric(cursor++, 0xF2, 0, (uls << 2) + offsetS, (ult << 2) + offsetT, lrs << 2, lrt << 2);
    } else {
        gSetTextureImage(cursor++, texture->format, texture->size, texture->width, texture->pixels_1C);
        gSetTile(cursor++, texture->format, texture->size, ((((lrs - uls + 1) * loadBytes) + 7) >> 3), 0, 7, 0,
                 texture->wrapT, maskT, 0, texture->wrapS, maskS, 0);
        gNoParam(cursor++, 0xE6);
        gTileGeneric(cursor++, 0xF4, 7, uls << 2, ult << 2, lrs << 2, lrt << 2);
        gNoParam(cursor++, 0xE7);
        gSetTile(cursor++, texture->format, texture->size, ((((lrs - uls + 1) * renderBytes) + 7) >> 3), 0, 0, 0,
                 texture->wrapT, maskT, 0, texture->wrapS, maskS, 0);
        gTileGeneric(cursor++, 0xF2, 0, (uls << 2) + offsetS, (ult << 2) + offsetT, lrs << 2, lrt << 2);
    }
    return cursor;
}

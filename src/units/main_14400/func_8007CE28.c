#include "common.h"
typedef unsigned char u8;
typedef signed char s8;
typedef float f32;
typedef struct { u32 w0, w1; } Gfx;
typedef struct { s8 r, g; u8 b; u8 pad; } Color;
typedef struct { Color fg; Color bg; } BarColors;

#define SHIFTL(v, s, w) ((((u32)(v)) & ((0x01 << (w)) - 1)) << (s))
#define GPACK_RGBA5551(r, g, b, a) ((((r) << 8) & 0xF800) | (((g) << 3) & 0x7C0) | (((b) >> 2) & 0x3E) | ((a) & 0x1))
#define GFX2(pkt, a, b) { Gfx *_g = (Gfx *)(pkt); _g->w0 = (a); _g->w1 = (b); }
#define gDPPipeSync(pkt) GFX2(pkt, 0xE7000000, 0)
#define gDPSetCycleType(pkt, type) GFX2(pkt, 0xE3000A01, type)
#define gSPTexture(pkt, s, t, on) GFX2(pkt, 0xD7000000 | SHIFTL(on, 1, 7), SHIFTL(s, 16, 16) | SHIFTL(t, 0, 16))
#define gDPSetRenderMode(pkt, mode) GFX2(pkt, 0xE200001C, mode)
#define gDPSetFillColor(pkt, c) GFX2(pkt, 0xF7000000, c)
#define gDPFillRectangle(pkt, ulx, uly, lrx, lry) \
    GFX2(pkt, 0xF6000000 | SHIFTL(lrx, 14, 10) | SHIFTL(lry, 2, 10), SHIFTL(ulx, 14, 10) | SHIFTL(uly, 2, 10))

extern s32 D_8013DE9C;
extern BarColors D_8013DF4C[];

Gfx *func_8007CE28(Gfx *gdl, s32 cur, s32 max){
  s32 scaled = 0; s32 r1, g1, r2, g2; u32 b1, b2;
  s32 idx = D_8013DE9C;
  if (max > 170) {
    if (idx == 0) idx = 2;
    scaled = 1;
  }
  if (scaled == 1) {
    cur = (f32)cur / (f32)max * 170.0f;
    max = 170;
  }
  r1 = D_8013DF4C[idx].fg.r;
  g1 = D_8013DF4C[idx].fg.g;
  b1 = D_8013DF4C[idx].fg.b;
  r2 = D_8013DF4C[idx].bg.r;
  g2 = D_8013DF4C[idx].bg.g;
  b2 = D_8013DF4C[idx].bg.b;
  gDPPipeSync(gdl++);
  gDPSetCycleType(gdl++, 0x300000);
  gSPTexture(gdl++, 0x8000, 0x8000, 0);
  gDPSetRenderMode(gdl++, 0);
  gDPSetFillColor(gdl++, 0xFFFFFFFF);
  gDPFillRectangle(gdl++, 123, 32, max + 125, 36);
  gDPPipeSync(gdl++);
  gDPSetFillColor(gdl++, GPACK_RGBA5551(r1, g1, b1, 1) << 16 | GPACK_RGBA5551(r1, g1, b1, 1));
  gDPFillRectangle(gdl++, 124, 33, cur + 124, 35);
  gDPPipeSync(gdl++);
  gDPSetFillColor(gdl++, GPACK_RGBA5551(r2, g2, b2, 1) << 16 | GPACK_RGBA5551(r2, g2, b2, 1));
  gDPFillRectangle(gdl++, cur + 125, 33, max + 123, 35);
  gDPPipeSync(gdl++);
  gSPTexture(gdl++, 0x8000, 0x8000, 1);
  return gdl; }

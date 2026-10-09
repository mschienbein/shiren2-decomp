#include "common.h"
typedef unsigned char u8; typedef unsigned short u16; typedef signed char s8; typedef short s16; typedef float f32;
typedef struct { u32 w0, w1; } Gfx;

#define GFX_CMD(pkt, a, b) { Gfx *_g = (Gfx *)(pkt); _g->w0 = (a); _g->w1 = (b); }

Gfx *func_80060230(Gfx *g, const void *timg, s32 fmt, u32 width, u32 height, s32 x, s32 y, f32 sx, f32 sy){
  u8 type = fmt & 7;
  u8 siz = fmt & 0x30;
  u32 tmem;
  s32 skipX, skipY, x0, y0, cols, rows;
  f32 f;
  u32 im;
  u32 line;
  u32 words;
  u32 step;
  s32 dsdx, dtdy;
  u32 i;

  tmem = 0x1000;
  if (fmt & 4) tmem = 0x800;

  skipX = 0;
  if (x >= 0) {
    x0 = x;
  } else {
    f = (f32)-x / sx;
    skipX = (u32)f;
    if ((f32)skipX < f) skipX++;
    skipX = (skipX + 3) & ~3;
    x0 = (f32)x + (f32)skipX * sx;
  }
  f = (f32)x + (f32)width * sx * 4.0f;
  if (f > 1280.0f) f = 1280.0f;
  f -= (f32)x0;
  cols = 0;
  if (!(f < 0.0f)) cols = ((s32)(f / sx) + 3) & ~3;

  skipY = 0;
  if (y >= 0) {
    y0 = y;
  } else {
    f = (f32)-y / sy;
    skipY = (u32)f;
    if ((f32)skipY < f) skipY++;
    skipY = (skipY + 3) & ~3;
    y0 = (f32)y + (f32)skipY * sy;
  }
  f = (f32)y + (f32)height * sy * 4.0f;
  if (f > 960.0f) f = 960.0f;
  f -= (f32)y0;
  rows = 0;
  if (!(f < 0.0f)) rows = ((s32)(f / sy) + 3) & ~3;

  if (cols == 0 || rows == 0) return g;

  switch (type) {
  case 0: im = 4; break;
  case 1: im = 3; break;
  case 4: case 5: im = 2; break;
  default: im = 0; break;
  }
  switch (siz) {
  case 0: case 0x10: case 0x20: break;
  default: siz = 0x30; break;
  }
  switch (siz) {
  case 0:
    line = (((cols + 11) >> 3) + 7) & ~7; words = line >> 3;
    GFX_CMD(g++, 0xFD080000 | (im << 21) | ((((width + 1) >> 1) - 1) & 0xFFF), (u32)timg);
    GFX_CMD(g++, 0xF5080000 | (im << 21) | ((words & 0x1FF) << 9), 0x07080200);
    GFX_CMD(g++, 0xF5000000 | (im << 21) | ((words & 0x1FF) << 9), 0x00080200);
    break;
  case 0x10:
    line = (((cols + 4) >> 2) + 7) & ~7; words = line >> 3;
    GFX_CMD(g++, 0xFD080000 | (im << 21) | ((width - 1) & 0xFFF), (u32)timg);
    GFX_CMD(g++, 0xF5080000 | (im << 21) | ((words & 0x1FF) << 9), 0x07080200);
    GFX_CMD(g++, 0xF5080000 | (im << 21) | ((words & 0x1FF) << 9), 0x00080200);
    break;
  case 0x20:
    line = (((cols + 4) >> 1) + 7) & ~7; words = line >> 3;
    GFX_CMD(g++, 0xFD100000 | (im << 21) | ((width - 1) & 0xFFF), (u32)timg);
    GFX_CMD(g++, 0xF5100000 | (im << 21) | ((words & 0x1FF) << 9), 0x07080200);
    GFX_CMD(g++, 0xF5100000 | (im << 21) | ((words & 0x1FF) << 9), 0x00080200);
    break;
  default:
    line = (cols + 11) & ~7; words = (((cols + 4) >> 1) + 7) >> 3;
    GFX_CMD(g++, 0xFD180000 | (im << 21) | ((width - 1) & 0xFFF), (u32)timg);
    GFX_CMD(g++, 0xF5180000 | (im << 21) | ((words & 0x1FF) << 9), 0x07080200);
    GFX_CMD(g++, 0xF5180000 | (im << 21) | ((words & 0x1FF) << 9), 0x00080200);
    GFX_CMD(g++, 0xFD180000 | (im << 21) | ((width - 1) & 0xFFF), (u32)timg);
    break;
  }
  step = ((tmem / line) - 1) * 4;
  dsdx = (1.0f / sx) * 1024.0f;
  dtdy = (1.0f / sy) * 1024.0f;
  for (i = 0; i < rows; i += step) {
    u32 n = rows - i;
    if (step < n) n = step;
    if (siz == 0) {
      GFX_CMD(g++, 0xE6000000, 0);
      GFX_CMD(g++, 0xF4000000 | (((skipX >> 1) & 0xFFF) << 12) | ((skipY + i) & 0xFFF),
              0x07000000 | (((((skipX + cols) >> 1) - 1) & 0xFFF) << 12) | ((skipY + i + n) & 0xFFF));
    } else {
      GFX_CMD(g++, 0xE6000000, 0);
      GFX_CMD(g++, 0xF4000000 | ((skipX & 0xFFF) << 12) | ((skipY + i) & 0xFFF),
              0x07000000 | (((skipX + cols - 1) & 0xFFF) << 12) | ((skipY + i + n) & 0xFFF));
    }
    GFX_CMD(g++, 0xE7000000, 0);
    if (n == rows - i) {
      GFX_CMD(g++, 0xF2000000, (((cols - 4) & 0xFFF) << 12) | ((n - 4) & 0xFFF));
    } else {
      GFX_CMD(g++, 0xF2000000, (((cols - 4) & 0xFFF) << 12) | (n & 0xFFF));
    }
    GFX_CMD(g++, 0xE4000000 | (((u32)((f32)x0 + (f32)cols * sx) & 0xFFF) << 12) | ((u32)((f32)y0 + (f32)(i + n) * sy) & 0xFFF),
            ((x0 & 0xFFF) << 12) | ((u32)((f32)y0 + (f32)i * sy) & 0xFFF));
    GFX_CMD(g++, 0xE1000000, 0);
    GFX_CMD(g++, 0xF1000000, ((dsdx & 0xFFFF) << 16) | (dtdy & 0xFFFF));
  }
  return g; }

#include "common.h"
typedef unsigned char u8; typedef unsigned short u16; typedef signed char s8; typedef short s16;
typedef struct { u32 w0, w1; } Gfx;
Gfx *func_8007CCBC(Gfx *g){
  { Gfx *c = g++; c->w0 = 0xD7000000; c->w1 = 0x80008000; }
  { Gfx *c = g++; c->w0 = 0xE2001E01; c->w1 = 0; }
  { Gfx *c = g++; c->w0 = 0xE200001C; c->w1 = 0x0F0A4000; }
  { Gfx *c = g++; c->w0 = 0xFCFFFFFF; c->w1 = 0xFFFE793C; }
  return g;
}

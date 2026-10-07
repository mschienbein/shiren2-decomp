#include "common.h"
typedef unsigned char u8; typedef unsigned short u16; typedef signed char s8; typedef short s16;
typedef struct { s16 delta; s16 idx; void (*fn)(void*, s32, u8*); } VTE;
typedef struct { u8 pad[0x28]; VTE e28; } VT;
typedef struct { s32 x0; s32 x4; u8 pad[0x10]; VT *vt; } Obj;
extern void func_800CA0A8(Obj*, s32); extern void func_800CA0EC(Obj*, s32*, u8);
s32 func_800CA118(Obj *o){ u8 b; s32 magic = 0x4368758A; s32 saved = o->x0; s32 n;
  func_800CA0A8(o, 8);
  n = o->x4 - o->x0;
  for (;;) { s32 c = n--; if (c <= 0) break; o->vt->e28.fn((u8*)o + o->vt->e28.delta, 1, &b); func_800CA0EC(o, &magic, b); }
  func_800CA0A8(o, saved);
  return magic; }

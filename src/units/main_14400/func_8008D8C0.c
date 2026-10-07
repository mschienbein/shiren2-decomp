#include "common.h"
typedef unsigned char u8; typedef unsigned short u16; typedef signed char s8; typedef short s16;
typedef struct { s32 x0; s32 x4; } Ent;
typedef struct { u8 pad[0xC]; u32 xC; Ent *x10; } Tbl;
extern Ent *func_80091450(u32); extern s32 func_8008DF04(void*); extern u8 *func_8006A810(void*, s32, s32); extern void func_80091544(void*);
s32 func_8008D8C0(void *ctx, Tbl *t){ s32 err = 0; s32 size = 0; u32 i;
  if (t->xC != 0) {
    u32 n = t->xC * 8;
    t->x10 = func_80091450(n);
    if (t->x10 == 0) { err = -1; } else { func_8006A810(t->x10, 0, n); for (i = 0; i < t->xC; i++) { t->x10[i].x0 = func_8008DF04(ctx); size += 4; } }
  }
  if (err) { size = -1; if (t->x10) { func_80091544(t->x10); t->x10 = 0; } }
  return size; }

#include "common.h"
typedef unsigned char u8;
typedef unsigned short u16;
typedef struct { u8 value; } Dir;
typedef struct { s32 a, b; } Pos;
extern void *func_800A2594(void *out, void *from, Dir dir);
extern void *func_800B4D80(Pos *);
extern u32 func_800B1C6C(void *pos);
extern s32 func_800A6E50(void *arg0, void *arg1);
extern void *D_80147FE0;
s32 func_800D5670(void *a, u8 *b){ s32 i = -2;
  for (;;) { Pos t; void *p; s32 found; Dir d;
    if (!(i < 3)) break;
    d.value = (*b + i) & 7;
    func_800A2594(&t, a, d);
    found = 0;
    p = func_800B4D80(&t);
    if (p != 0 && !(func_800B1C6C(&t) & 0x4000)) found = func_800A6E50(D_80147FE0, p) != 0;
    if (found) return 0;
    i++;
  }
  return 1; }

#include "common.h"
typedef unsigned char u8;
typedef unsigned short u16;
extern s32 func_800B502C(void *);
extern void *func_800B4D80(void *);
extern u32 func_800B1C6C(void *);
extern s32 func_800A4010(void *, u16, s32);
typedef struct { char pad[0x1D]; u8 x1D; u8 x1E; } Obj;
s32 func_800A422C(Obj *o, void *a, s32 b){
  if (o->x1D >> 7) { if (func_800B502C(a)) return 0; }
  if ((o->x1E >> 2) & 1) { u8 *p = func_800B4D80(a); if (p != 0 && p[1] == 0xF4) return 0; }
  return func_800A4010(o, func_800B1C6C(a), b); }

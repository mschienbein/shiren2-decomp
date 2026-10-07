#include "common.h"
typedef unsigned char u8; typedef unsigned short u16; typedef signed char s8; typedef short s16; typedef float f32;
typedef struct { s16 delta; s16 idx; void (*fn)(void*, s32); } VTE;
typedef struct { u8 pad[8]; VTE e8; } VT;
typedef struct { u8 x0; u8 pad[3]; u8 x4; u8 pad5[3]; VT *vt; } Item;
typedef struct { u8 x0; u8 pad[4]; s8 x5; } Ent;
typedef struct { u8 pad[0x10]; } Iter;
extern s32 func_800CD278(void*); extern Iter *func_800CEB20(Iter*, void*); extern s32 func_800CEBA0(Iter*); extern Ent *func_800CEC68(Iter*);
extern s32 func_800AE9AC(Ent*, s32, s32); extern s32 func_800A2A24(Ent*, Item*); extern void func_800D3698(s32, s32);
extern s32 func_800CD114(void*, s32); extern void func_800D3650(Item*); extern s32 func_800CD5C0(void*, Item*);
s32 func_800CD61C(void *a, Item *item, s32 any){
  Iter it;
  s32 enough = func_800CD278(a) >= item->x4;
  if (!enough) return 0;
  func_800CEB20(&it, a);
  while (func_800CEBA0(&it)) {
    Ent *e = func_800CEC68(&it);
    s32 before = func_800AE9AC(e, 0, 0);
    s32 ok = 0;
    if ((any || e->x0 == item->x0) && func_800A2A24(e, item)) ok = 1;
    if (ok) {
      if (~e->x5) func_800D3698(e->x5, before - func_800AE9AC(e, 0, 0));
      func_800CD114(a, -item->x4);
      func_800D3650(item);
      if (item) item->vt->e8.fn((u8*)item + item->vt->e8.delta, 3);
      return 1;
    }
  }
  func_800CD5C0(a, item);
  return 0; }

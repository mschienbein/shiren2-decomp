#include "common.h"
typedef unsigned char u8; typedef unsigned short u16; typedef signed char s8; typedef short s16; typedef float f32;
typedef struct { s16 delta; s16 idx; void *(*fn)(void*); } VTE;
typedef struct { u8 pad[0x98]; VTE e98; } VT;
typedef struct { u8 hi:5; u8 active:1; u8 lo:2; } TgtFlags;
typedef struct { u8 pad[0x1E]; TgtFlags x1E; u8 pad1F[5]; VT *vt; } Tgt;
typedef struct { s32 x0; Tgt *x4; u8 pad[0x10]; s32 x18; } Msg;
typedef struct { u8 x0; u8 x1; u8 x2; } Obj;
typedef struct { u8 pad[0x10]; } Iter;
extern u32 D_8013960C;
extern s32 func_800AF28C(Obj*, Msg*); extern s32 func_80049CB4(s32, ...); extern char *func_800AC990(Obj*); extern void func_800498E4(s32, ...);
extern void *func_800CEB20(Iter*, void*); extern s32 func_800CEBA0(Iter*); extern Obj *func_800CEC68(Iter*); extern void func_800AE518(Obj*, Tgt*, s32, s32);
static inline s32 flags_active(TgtFlags *f){ return f->active; }
s32 func_8010C96C(Obj *o, Msg *m){
  if (m->x0 == 15) {
    s32 show = D_8013960C & 1; Iter it; s32 inactive; Tgt *t;
    inactive = flags_active(&m->x4->x1E) != 1;
    if (inactive) return 0;
    t = m->x4;
    if (m->x18 == 0) {
      if (!(o->x2 & 4)) return 1;
      o->x2 &= ~4;
      if (show) { func_80049CB4(0xC7, o); func_800498E4(0x2C, func_800AC990(o)); }
      return 1;
    }
    func_800CEB20(&it, t->vt->e98.fn((u8*)t + t->vt->e98.delta));
    while (func_800CEBA0(&it)) {
      Obj *e = func_800CEC68(&it);
      if (e == o) continue;
      switch (e->x0) {
      case 5: case 11: case 12: case 13:
        D_8013960C = (D_8013960C << 1) & ~1;
        func_800AE518(e, t, 0, 0);
        D_8013960C >>= 1;
        break;
      }
    }
    o->x2 |= 4;
    if (show) { func_80049CB4(0xC6, o); func_800498E4(0x2B, func_800AC990(o)); }
    return 1;
  }
  return func_800AF28C(o, m); }

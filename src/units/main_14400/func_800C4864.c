#include "common.h"
typedef unsigned char u8; typedef unsigned short u16; typedef signed char s8; typedef short s16; typedef float f32;
typedef struct { s32 x, y; } Pos;
typedef struct { Pos pos; u8 x8; u8 x9; u8 pad[0x12]; u16 x1C; } Actor;
typedef struct { u8 dir; u8 face; } Dir;
typedef struct { s16 delta; s16 idx; void *fn; } VTE;
typedef struct { VTE e0; VTE e8; VTE e10; VTE e18; VTE e20; VTE e28; } VT;
typedef struct { Actor *actor; u16 x4; u16 x6; s32 x8; VT *vt; } Move;
typedef s32 (*FnPP)(void*, Pos*, Pos*); typedef char *(*Fn0)(void*); typedef s32 (*FnAA)(void*, Actor*, Actor*);
typedef void (*FnHit)(void*, Actor*, u8*, Actor*); typedef void (*FnMiss)(void*, Actor*, u8*, Pos*);
extern s32 func_800A251C(Pos*, Pos*); extern Dir *func_800A22B8(Dir*, Pos*, Pos*); extern Actor *func_800B4928(Pos*);
extern void func_800A665C(Actor*, u8*); extern s32 func_80049CB4(s32, ...); extern u32 func_800B1C6C(Pos*); extern s32 func_800A58B8(Actor*);
extern s32 func_800A5440(Actor*, Actor*, u8*, s32, char*); extern s32 func_800A6FD0(Actor*); extern s32 func_8005DFE8(u8*);
extern char *func_800A3B20(Actor*); extern void func_800497F0(s32, ...);
static inline void pos_copy(Pos *d, Pos *s){ d->x = s->x; d->y = s->y; }
static inline void face_toward(Actor *a, Pos *from, Pos *to, Dir *d){ s32 failed = func_800A251C(from, to) != 1; if (failed) { func_800A22B8(d, from, to); d->face = d->dir; } else d->face = a->x8; }
void func_800C4864(Move *self, s32 sfx, Pos *target){
  Actor *a = self->actor; Pos from; Pos to; Dir d; Actor *tgt; s32 res; s32 power;
  pos_copy(&from, &a->pos);
  to.x = target->x; to.y = target->y;
  face_toward(a, &from, &to, &d);
  tgt = func_800B4928(&to);
  if (tgt && (tgt->x1C & 1)) tgt = 0;
  res = 1;
  if (sfx) { func_800A665C(a, &d.face); func_80049CB4(sfx | 0x1000, a); }
  power = ((FnPP)self->vt->e8.fn)((u8*)self + self->vt->e8.delta, &from, &to);
  if (tgt) {
    if (tgt == a) tgt = 0;
    else {
      s32 skip = 0;
      if (!(func_800B1C6C(&to) & 0x2000) && (tgt->x9 & 0xF) == 1) skip = func_800A58B8(tgt) == 1;
      if (skip) tgt = 0;
    }
    if (tgt) {
      if (!(self->x4 & 0x10)) {
        if (((FnAA)self->vt->e28.fn)((u8*)self + self->vt->e28.delta, a, tgt)) return;
      }
      res = func_800A5440(tgt, a, &d.face, self->x4, ((Fn0)self->vt->e10.fn)((u8*)self + self->vt->e10.delta));
      if (res == 2) {
        Actor *t = a; a = tgt; tgt = t;
        d.face = (d.face + 4) & 7;
        from = a->pos; to = tgt->pos;
        func_800A665C(a, &d.face);
        power = ((FnPP)self->vt->e8.fn)((u8*)self + self->vt->e8.delta, &from, &to);
        self->x4 |= 0x10;
        res = func_800A5440(tgt, a, &d.face, self->x4, ((Fn0)self->vt->e10.fn)((u8*)self + self->vt->e10.delta));
      }
    }
  }
  func_80049CB4(2);
  func_80049CB4(0x131);
  if (res == 0) {
    if (func_800A6FD0(tgt)) power = -1;
    if (self->x8) {
      char *lvl;
      func_80049CB4(6);
      lvl = ((Fn0)self->vt->e10.fn)((u8*)self + self->vt->e10.delta);
      if (func_8005DFE8(lvl) < 0x34) func_800497F0(0x63, power, lvl, func_800A3B20(tgt));
      else func_800497F0(0x64, power, lvl, func_800A3B20(tgt));
      func_80049CB4(7);
    }
    ((FnHit)self->vt->e18.fn)((u8*)self + self->vt->e18.delta, a, &d.face, tgt);
  } else {
    ((FnMiss)self->vt->e20.fn)((u8*)self + self->vt->e20.delta, a, &d.face, &to);
  } }

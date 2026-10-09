#include "common.h"
typedef unsigned char u8;
typedef unsigned short u16;
typedef struct { u8 v; } Dir;
typedef struct { s32 x, y; } Pos;
typedef struct {
    Pos pos;
    u8 dir;
    char pad9[0x13];
    u16 flags1C;
    u8 bits1E;
    char pad1F[0x35];
    u8 state54;
    char pad55[3];
    void *target;
} Mon;
extern char D_80147620[];
extern s32 func_800E1D14(Mon *, s32);
extern s32 func_800E1CC4(Mon *, s32);
extern s32 func_800E1CD4(Mon *, s32);
extern s32 func_800E2044(Mon *);
extern u8 func_800C57CC(void *, s32);
extern u8 func_800C57A0(void *);
extern s32 func_800A4EFC(Mon *, Dir *);
extern void func_800A665C(Mon *, Dir *);
extern void *func_800A6CF0(Mon *);
extern s32 func_800A455C(Mon *, void *, s32);
extern Dir *func_800A7DE4(Mon *);
extern s32 func_800A50E8(Mon *);
extern void *func_800A65E4(Dir *, Mon *, void *);
extern s32 func_800E7104(Mon *);
extern s32 func_800E66EC(Mon *);
extern s32 func_80049CB4(s32, ...);
extern u8 func_800A6420(Mon *, void *);
extern Dir *func_800A22B8(Dir *, Pos *, void *);

static inline void acquire(Mon *m){
  void *found = func_800A6CF0(m);
  if (func_800A455C(m, found, 1)) {
    m->target = found;
    m->state54 |= 4;
  }
}

static inline void face(Mon *m, Dir *d, s32 delta){ d->v = (m->dir + delta) & 7; func_800A665C(m, d); }

s32 func_800E8350(Mon *m){
  Pos pos; Pos *pp; Dir d0; Dir d1; Dir d2; Dir d3; Dir d4; Dir *pd; void *target; s32 flag;
  if (func_800E1D14(m, 0x13)) {
    d0.v = func_800C57CC(D_80147620, 7) & 7;
    {
      Dir *dir = &d0;
      if (func_800E2044(m) && func_800A4EFC(m, dir)) return 1;
    }
    func_800A665C(m, &d0);
    acquire(m);
    return 0;
  }
  if (func_800E1CC4(m, 0)) {
    if (func_800E2044(m) && func_800A4EFC(m, func_800A7DE4(m))) return 1;
    {
      void *t = func_800A6CF0(m);
      if (func_800A455C(m, t, 1)) {
        m->target = t;
        m->state54 |= 4;
        return 0;
      }
    }
    face(m, &d1, func_800C57CC(D_80147620, 2) - 1);
    return 0;
  }
  flag = 0;
  if (func_800E1CC4(m, 1) && !((m->bits1E >> 2) & 1)) flag = 1;
  else if (func_800E1D14(m, 0x14)) flag = 1;
  if (flag) {
    flag = 0;
    if (func_800E2044(m)) flag = 1;
    else if (m->flags1C & 2) flag = 1;
    if (!flag) return 0;
    if (func_800C57A0(D_80147620) & 1) {
      if (m->flags1C & 2) {
        face(m, &d2, func_800C57CC(D_80147620, 2) - 1);
        return 0;
      }
      return func_800A50E8(m);
    }
    if (m->flags1C & 2) {
      if (func_800A455C(m, m->target, 1)) {
        m->state54 |= 4;
        pd = &d3;
        func_800A65E4(pd, m, m->target);
        func_800A665C(m, pd);
      }
      return 0;
    }
    if ((m->bits1E >> 4) & 1) return func_800E7104(m);
    return func_800E66EC(m);
  }
  if (!func_800E1CD4(m, 0x10)) return 0;
  pp = &pos;
  pp->x = m->pos.x;
  pp->y = m->pos.y;
  func_80049CB4(0xEA, &pos);
  target = m->target;
  switch (func_800A6420(m, target)) {
  case 0:
    m->state54 |= 4;
  case 1:
  case 2:
    pd = &d4;
    func_800A22B8(pd, &pos, target);
    break;
  default:
    return 0;
  }
  func_800A665C(m, pd);
  return 0; }
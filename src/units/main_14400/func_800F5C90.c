#include "common.h"
typedef unsigned char u8; typedef unsigned short u16; typedef signed char s8; typedef short s16; typedef float f32;
typedef struct { u32 hi:8; u32 hidden:1; u32 lo:23; } Flags;
typedef struct { s16 delta; s16 idx; s32 (*fn)(void*, s32, s32, u8, s32); } VTE;
typedef struct { u8 pad[0x90]; VTE e90; } VT;
typedef struct { u8 pad[0x20]; Flags x20; VT *vt; } Ent;
typedef struct { u8 pad[0x2C]; u8 x2C; } Obj;
typedef struct { u8 pad[0x20]; } Iter;
extern u8 D_80147620[]; extern u8 D_80156A41; extern u32 D_8013960C;
extern s32 func_800C587C(void*, u8); extern s32 func_80049CB4(s32, ...); extern u32 func_800B1C6C(void*); extern char *func_800A3B20(Obj*);
extern void func_800497F0(s32, ...); extern Iter *func_800A915C(Iter*, Obj*); extern s32 func_800A9284(Iter*, s32); extern Ent *func_800A942C(Iter*);
extern s32 func_800A58B8(Ent*);
static inline s32 flags_hidden(Flags *f){ return f->hidden; }
static inline s32 test_bits(u32 v, u32 m){ if (v & m) return 1; return 0; }
void func_800F5C90(Obj *o){
  Iter it; s32 failed; s32 h;
  failed = func_800C587C(D_80147620, D_80156A41) != 1;
  if (failed || o->x2C < 5) { o->x2C++; return; }
  o->x2C = 0;
  func_80049CB4(0x1131);
  func_80049CB4(6);
  h = func_80049CB4(0xB7, o, test_bits(func_800B1C6C(o), 0x1000));
  func_80049CB4(7);
  func_800497F0(0x114, h, func_800A3B20(o));
  func_80049CB4(0x132);
  D_8013960C <<= 1;
  func_800A915C(&it, o);
  while (func_800A9284(&it, 0x7C)) {
    Ent *e = func_800A942C(&it); s32 ok = 0;
    if (!(func_800B1C6C(e) & 0x4000) && func_800A58B8(e) != 1) { Flags f = e->x20; ok = !flags_hidden(&f); }
    if (!ok) continue;
    func_80049CB4(6);
    e->vt->e90.fn((u8*)e + e->vt->e90.delta, 0, 10, 0xFE, 0);
    func_80049CB4(7);
  }
  D_8013960C >>= 1; }

#include "common.h"
typedef unsigned char u8; typedef unsigned short u16; typedef signed char s8; typedef short s16; typedef float f32;
typedef struct { s8 d; } Dir;
typedef struct { s32 a, b; } Id;
typedef struct { Id id; u8 x8; } Obj;
typedef struct { u8 pad[0x18]; Id *cur; } Iter;
typedef union { Iter it; struct { Id out; Id key; } fb; } Scratch;
extern Id *func_800F1A58(Obj*, s32, s32); extern Iter *func_800C2B40(Iter*, Obj*, u8*, s32); extern s32 func_800C2BBC(Iter*, u8);
extern s32 func_800A674C(Obj*, Id*); extern s32 func_800A6E90(Id*); extern void *func_800B5A18(Id*, Id*, Dir, s32, u16);
static inline s32 id_is_null(Id *p){ return (p->b | p->a) == 0; }
static inline void id_copy(Id *dst, Id *src){ dst->a = src->a; dst->b = src->b; }
Id *func_800F1B28(Id *ret, Obj *o, s32 a2, s32 a3, s32 a4){
  Id res; Id *found; Scratch s;
  res.a = 0; res.b = 0;
  found = func_800F1A58(o, a2, a3);
  if (found) { res = *found; }
  else if (!a4) {
    func_800C2B40(&s.it, o, &o->x8, a2);
    while (func_800C2BBC(&s.it, 0xFF)) {
      s32 ok = 0;
      found = s.it.cur;
      if ((a3 || func_800A674C(o, found)) && !func_800A6E90(found)) ok = 1;
      if (ok) { res = *found; break; }
    }
  }
  if (id_is_null(&res)) { Dir dir; id_copy(&s.fb.key, &o->id); dir.d = o->x8; func_800B5A18(&s.fb.out, &s.fb.key, dir, a2, (u16)0xC000); res = s.fb.out; }
  id_copy(ret, &res);
  return ret; }

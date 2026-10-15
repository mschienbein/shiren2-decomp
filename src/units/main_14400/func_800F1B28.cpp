#include "common.h"
#include "map_types.h"
typedef unsigned char u8; typedef unsigned short u16; typedef signed char s8;
/* Object id with a user-declared member-wise copy constructor (as in func_800B3024.cpp). */
struct Id {
  s32 a, b;
  Id() {}
  Id(const Id &other) : a(other.a), b(other.b) {}
};
/* Views of the object (Id at +0, facing byte at +8) and of its 0x1C-byte iterator (+0x18 cursor). */
struct Obj_800F1B28 { Id id; u8 x8; };
struct Iter_800F1B28 { u8 pad[0x18]; Id *cur; };
extern "C" {
Id *func_800F1A58(Obj_800F1B28*, s32, s32); Iter_800F1B28 *func_800C2B40(Iter_800F1B28*, Obj_800F1B28*, u8*, s32); s32 func_800C2BBC(Iter_800F1B28*, u8);
s32 func_800A674C(Obj_800F1B28*, Id*); s32 func_800A6E90(Id*); Id func_800B5A18(Id*, MapDir, s32, u16);
}
static inline s32 id_is_null(Id *p){ return (p->b | p->a) == 0; }
static inline void id_copy(Id *dst, Id *src){ dst->a = src->a; dst->b = src->b; }
/* Trace from a copy of the object's position along its facing (func_800B5A18 moves its
   argument in place). */
static inline Id trace_from(Obj_800F1B28 *o, s32 range) {
  Id start; MapDir dir;
  id_copy(&start, &o->id); dir.value = o->x8;
  return func_800B5A18(&start, dir, range, (u16)0xC000);
}
extern "C" Id *func_800F1B28(Id *ret, Obj_800F1B28 *o, s32 a2, s32 a3, s32 a4){
  Id res; Id *found;
  res.a = 0; res.b = 0;
  found = func_800F1A58(o, a2, a3);
  if (found) { res = *found; }
  else if (!a4) {
    Iter_800F1B28 it;
    func_800C2B40(&it, o, &o->x8, a2);
    while (func_800C2BBC(&it, 0xFF)) {
      s32 ok = 0;
      found = it.cur;
      if (a3 || func_800A674C(o, found)) { if (!func_800A6E90(found)) ok = 1; }
      if (ok) { res = *found; break; }
    }
  }
  if (id_is_null(&res)) res = trace_from(o, a2);
  id_copy(ret, &res);
  return ret; }

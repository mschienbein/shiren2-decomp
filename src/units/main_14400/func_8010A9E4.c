#include "common.h"
typedef unsigned char u8;
typedef struct { s32 x, y; } Pos;
typedef struct { Pos lo, hi; } Rect;
typedef struct { Pos cur; Pos start; Pos end; } Iter;
typedef struct { char pad[0xCC]; Pos xCC; } Obj;
extern Pos *D_801476B8;
extern s32 func_800A650C(Obj *, Pos *);
extern void *func_800A2FD0(void *, void *, u8);
extern void *func_800A3610(void *, void *);
extern u32 func_800B1C6C(Pos *);
extern s32 func_800A251C(Pos *, Pos *);
extern void *func_800B4D80(Pos *);
extern s32 func_8010A944(Obj *, void *);
static inline void pos_set(Pos *dst, s32 x, s32 y){ Pos p; p.x = x; p.y = y; *dst = p; }
static inline void iter_init(Iter *it, Rect *r){ pos_set(&it->start, r->lo.x, r->lo.y); it->cur = it->start; pos_set(&it->end, r->hi.x, r->hi.y); }
void *func_8010A9E4(Obj *self, Pos *out){
  Pos best_pos; Rect r; Iter it; void *best = 0; s32 best_dist = 1000;
  if (func_800A650C(self, D_801476B8) < 4) {
    func_800A2FD0(&r, D_801476B8, 3);
    iter_init(&it, &r);
    for (;;) {
      Pos cur; void *p; s32 skip; s32 d; s32 more = it.cur.x <= it.end.x;
      if (!more) break;
      func_800A3610(&cur, &it);
      skip = 0;
      if (!(func_800B1C6C(&cur) & 0x2000) || func_800A251C(&cur, &self->xCC)) skip = 1;
      if (skip) continue;
      p = func_800B4D80(&cur);
      if (p == 0) continue;
      if (!func_8010A944(self, p)) continue;
      d = func_800A650C(self, &cur);
      if (d < best_dist) { best = p; best_pos = cur; best_dist = d; }
    }
    if (best) *out = best_pos;
  }
  return best; }

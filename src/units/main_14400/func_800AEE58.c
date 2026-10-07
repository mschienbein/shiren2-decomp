#include "common.h"
typedef struct { short delta; short idx; void (*fn)(void *, s32, void *); } VEnt;
typedef struct { VEnt e[6]; } VT;
typedef struct { char pad[0x18]; VT *vt; } Obj;
static inline void send(Obj *o, s32 code, void *arg){ VEnt *e = &o->vt->e[5]; e->fn((char *)o + e->delta, code, arg); }
extern void func_800CA4E8(Obj *, void *);
extern char D_80153A30[], D_80147620[], D_80143084[], D_8014303C[], D_80143064[], D_80143044[];
extern void func_800C56D4(void *), func_800C5684(void *, void *), func_800AD650(void), func_800C573C(void *);
void func_800AEE58(Obj *self){ void *mgr; void *res;
  func_800CA4E8(self, D_80153A30);
  mgr = D_80147620;
  func_800C56D4(mgr);
  res = D_80143084;
  send(self, 0xC, res);
  func_800C5684(mgr, res);
  func_800AD650();
  func_800C573C(mgr);
  send(self, 1, D_8014303C);
  send(self, 0x20, D_80143064);
  send(self, 0x20, D_80143044); }

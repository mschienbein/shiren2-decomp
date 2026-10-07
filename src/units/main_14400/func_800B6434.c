#include "common.h"
typedef unsigned char u8;
typedef struct { short delta; short idx; void (*fn)(void *, s32, void *); } VEnt;
typedef struct { VEnt e[6]; } VT;
typedef struct { char pad[0x18]; VT *vt; } Obj;
typedef struct { u8 data[76]; } Rec;
typedef struct { s32 index; s32 offset; } Ref;
extern void func_800CA4E8(Obj *, void *);
extern char D_80153B2C[];
extern Rec D_80145460[];
void func_800B6434(Obj *self){ short count; short i; Ref ref; VEnt *e;
  func_800CA4E8(self, D_80153B2C);
  e = &self->vt->e[5];
  e->fn((char *)self + e->delta, 2, &count);
  for (i = 0; i < count; i++) {
    e = &self->vt->e[5];
    e->fn((char *)self + e->delta, 8, &ref);
    e = &self->vt->e[5];
    e->fn((char *)self + e->delta, 1, &D_80145460[ref.index].data[ref.offset]);
  } }

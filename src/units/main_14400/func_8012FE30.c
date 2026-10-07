#include "common.h"
typedef unsigned char u8; typedef unsigned short u16; typedef signed char s8; typedef short s16;
typedef struct Node { s32 x0; s32 x4; struct Node *x8; u8 pad[0x7C]; s32 x88; } Node;
typedef struct Msg { struct Msg *next; s32 x4; s16 x8; s16 xA; s32 xC; s32 x10; } Msg;
typedef struct { s32 x0; s32 x4; Node *x8; s32 xC; s32 x10; s16 x14; u16 x16; u16 x18; u16 x1A; } Obj;
typedef struct { s16 x0; u16 x2; u8 x4; } Src;
typedef struct { u8 pad[0x1C]; s32 x1C; } Glob;
extern Glob *D_80148D84;
extern s32 func_8012FF48(Node**, s16); extern Msg *func_80130780(void); extern s32 func_8012E5F8(Node*, s32, Msg*);
s32 func_8012FE30(Obj *o, Src *src){ Node *n; s32 r; Msg *m;
  o->x16 = src->x0; n = 0; o->x1A = src->x4; o->xC = 0; o->x18 = src->x2; o->x14 = 0; o->x8 = 0;
  r = func_8012FF48(&n, src->x0);
  if (n) {
    if (r) {
      n->x88 = 0x228; n->x8->x8 = 0; n->x8 = (Node*)o; o->x8 = n;
      m = func_80130780(); { s32 base = D_80148D84->x1C; m->x8 = 11; m->xC = 0; m->x10 = 0x170; m->x4 = base; }
      func_8012E5F8(o->x8, 3, m);
      m = func_80130780();
      if (m) { s32 pos = D_80148D84->x1C + n->x88; m->x8 = 15; m->next = 0; m->x4 = pos; func_8012E5F8(o->x8, 3, m); }
    } else { n->x88 = 0; n->x8 = (Node*)o; o->x8 = n; }
  }
  return n != 0; }

#include "common.h"

typedef struct { short delta; short index; void *(*fn)(void *); } VEntry;
typedef struct { char pad[0x98]; VEntry e; } VTable;
typedef struct { char pad[0x24]; VTable *vtbl; } Mgr;
typedef struct { char pad[0xC]; s32 xC; } Obj;
typedef struct { Obj *x0; s32 x4; s32 x8; } S;
extern Mgr *D_801476B8;
extern unsigned char D_80154718[];
signed char *func_800C9E00(void);
void func_800D1D78(void *, s32);
void func_800CD364(void *, Obj *);
void *func_800AC5B4(s32, s32);
Obj *func_80117080(void *, s32);
s32 func_800CD538(void *container, void *item);
void func_800A202C(S *s) {
    switch (s->x8) {
    case 1:
    case 2: {
        s32 t = s->x0->xC;
        func_800D1D78(func_800C9E00(), t);
        if (s->x8 == 2) {
            s->x0->xC = s->x4;
        } else {
            func_800CD364(D_801476B8->vtbl->e.fn((char *)D_801476B8 + D_801476B8->vtbl->e.delta), s->x0);
        }
        break;
    }
    case 3: {
        Obj *o;
        (func_800C9E00() + D_80154718[s->x4])[0xBB] = -1;
        o = func_80117080(func_800AC5B4(0x10, 0), 0xEE);
        o->xC = s->x4;
        func_800CD538(D_801476B8->vtbl->e.fn((char *)D_801476B8 + D_801476B8->vtbl->e.delta), o);
        break;
    }
    }
    s->x8 = 0;
}

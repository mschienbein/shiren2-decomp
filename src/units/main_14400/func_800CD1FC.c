#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;

typedef struct { u8 pad[4]; u8 f_4; } Item;
typedef struct Obj Obj;
typedef struct { s16 delta; s16 index; void *fn; } VEntry;
struct Obj { s32 f_0; VEntry *vt; };
typedef s32 (*CountFn)(void *self);
typedef Item *(*GetFn)(void *self, u32 i);
s32 func_800CD1FC(Obj *obj) {
    s32 sum = 0;
    s32 i = ((CountFn)obj->vt[4].fn)((u8 *)obj + obj->vt[4].delta) - 1;
    while (1) {
        if (i < 0) break;
        sum += ((GetFn)obj->vt[7].fn)((u8 *)obj + obj->vt[7].delta, i)->f_4;
        i--;
    }
    return sum;
}

#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;

typedef struct Obj Obj;
typedef struct { s16 delta; s16 index; void *fn; } VEntry;
struct Obj { s32 f_0; VEntry *vt; };
typedef s32 (*CountFn)(void *self);
typedef void *(*GetFn)(void *self, u32 i);
s32 func_800AEC2C(void *item);
s32 func_800CF3F0(Obj *obj) {
    s32 sum = 0;
    s32 i = ((CountFn)obj->vt[4].fn)((u8 *)obj + obj->vt[4].delta);
    while (1) {
        i--;
        if (i == -1) break;
        sum += func_800AEC2C(((GetFn)obj->vt[7].fn)((u8 *)obj + obj->vt[7].delta, i));
    }
    return sum;
}

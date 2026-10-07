#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;

extern s32 D_80159AA0;
typedef struct { s32 *table; s32 a; s32 b; s32 c; } Obj800F68F4;
Obj800F68F4 *func_800F68F4(Obj800F68F4 *obj, s32 a, s32 b, s32 c) {
    obj->table = &D_80159AA0;
    obj->a = a;
    obj->b = b;
    obj->c = c;
    return obj;
}

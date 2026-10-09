#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;

typedef s32 (*QueryFunc800E2EB4)(void *self, s32 arg1, s32 arg2, u8 arg3, s32 arg4);

typedef struct {
    s16 delta;
    s16 index;
    QueryFunc800E2EB4 func;
} VEntry800E2EB4;

typedef struct {
    u8 pad0[0x90];
    VEntry800E2EB4 query;
} VTable800E2EB4;

typedef struct {
    u8 pad0[0x24];
    VTable800E2EB4 *vtbl;
} Obj800E2EB4;

s32 func_800E2EB4(Obj800E2EB4 *obj)
{
    return obj->vtbl->query.func((u8 *)obj + obj->vtbl->query.delta, 1, 6, 0, 0);
}

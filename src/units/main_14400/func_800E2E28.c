#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;
typedef float f32;

/* Slot +0x94 targets func_800E115C / derived dispatchers; +0x90 is zero in their tables. */
typedef s32 (*QueryFunc800E2E28)(void *self, s32 arg1, s32 arg2, u8 arg3, s32 arg4);

typedef struct {
    s16 delta;
    s16 index;
    QueryFunc800E2E28 func;
} VEntry800E2E28;

typedef struct {
    u8 pad0[0x90];
    VEntry800E2E28 query;
} VTable800E2E28;

typedef struct {
    u8 pad0[0x24];
    VTable800E2E28 *vtbl;
} Obj800E2E28;

s32 func_800E2E28(Obj800E2E28 *obj) {
    return obj->vtbl->query.func((u8 *)obj + obj->vtbl->query.delta, 1, 7, 0, 0);
}

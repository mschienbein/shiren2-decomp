#include "common.h"

typedef unsigned char u8;
typedef short s16;

typedef struct {
    u8 pad0[0x20];
    s16 delta_20;
    s16 pad22;
    s32 (*func_24)(void *self);
} VTable800F3950;

typedef struct {
    u8 pad0[0x4];
    VTable800F3950 *vtable_4;
} Inner800F3950;

typedef struct {
    u8 pad0[0x8C];
    Inner800F3950 *inner_8C;
} Obj800F3950;

void func_800F3950(Obj800F3950 *obj) {
    Inner800F3950 *inner = obj->inner_8C;
    inner->vtable_4->func_24((u8 *)inner + inner->vtable_4->delta_20);
}

#include "common.h"

typedef unsigned char u8;
typedef signed short s16;
typedef struct {
    u8 pad00[0x90];
    s16 delta90;
    s16 index92;
    s32 (*query94)(void *, s32, s32, u8, s32);
} VTable;
typedef struct { u8 pad00[0x24]; VTable *vtable24; } Object;

s32 func_800E2E60(Object *self) {
    return self->vtable24->query94((u8 *)self + self->vtable24->delta90, 0, 7, 0xFE, 0);
}

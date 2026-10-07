#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;

/* Whole RNG object (state pointers plus backing words); only its base is passed here. */
extern u8 D_80147620[];
typedef struct { s16 delta; s16 index; s32 (*fn)(void *self, s32 arg); } VtblEntry800E547C;
typedef struct { u8 pad0[0x50]; VtblEntry800E547C entry; } Vtbl800E547C;
typedef struct { u8 pad0[0x24]; Vtbl800E547C *vtable; } Obj800E547C;
s32 func_800E2044(Obj800E547C *self);
s32 func_800C587C(void *rng, u8 limit);
s32 func_800E547C(Obj800E547C *self, Obj800E547C *other) {
    s32 value = 100;
    if (func_800E2044(self) != 0) {
        if (other != 0) {
            value = other->vtable->entry.fn((u8 *)other + other->vtable->entry.delta, 4);
        }
        if (func_800C587C(D_80147620, value) != 0) {
            value = self->vtable->entry.fn((u8 *)self + self->vtable->entry.delta, 0);
        } else {
            value = 100;
        }
    } else {
        value = 0;
    }
    return func_800C587C(D_80147620, value) ^ 1;
}

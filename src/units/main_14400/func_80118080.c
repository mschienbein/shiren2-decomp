#include "common.h"

typedef unsigned char u8;
typedef short s16;

typedef struct {
    u8 pad0[0x78];
    s16 delta_78;
    s16 pad7A;
    void (*func_7C)(void *self, s16 amount);
} VTable80118080;

typedef struct Obj_80049414 {
    u8 pad0[0x24];
    VTable80118080 *vtable_24;
} Obj_80049414;

extern s16 D_801569E6;

s32 func_800A99D0(void);
void func_800498E4(s32 id, ...);
s32 func_800E1CC4(Obj_80049414 *obj, s32 kind);

/* Item vtable slot +0x44: `self` is the adjusted receiver the dispatcher supplies; not used here. */
void func_80118080(void *self, Obj_80049414 *target)
{
    if (func_800A99D0() != 0) {
        func_800498E4(0x222);
    } else {
        s32 scale = func_800E1CC4(target, 3) != 0 ? 2 : 1;

        target->vtable_24->func_7C((u8 *)target + target->vtable_24->delta_78, D_801569E6 * scale);
    }
}

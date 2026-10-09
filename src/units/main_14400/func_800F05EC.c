#include "common.h"
typedef unsigned char u8;
typedef struct VTable {
    u8 pad_00[0x90];
    short adjustment_90, pad_92;
    s32 (*effect_94)(void *, s32, s32, u8, s32);
    short adjustment_98, pad_9A;
    void (*update_9C)(void *);
} VTable;
typedef struct Obj800EFC70 { u8 pad_00[0x24]; const VTable *vtable_24; } Obj800EFC70;
extern void func_800EFF0C(Obj800EFC70 *obj);
extern s32 func_800E1CC4(Obj800EFC70 *obj, s32 kind);
extern s32 func_800E4454(Obj800EFC70 *obj);
extern void func_800A7C1C(Obj800EFC70 *obj);
extern u32 D_8013960C;
void func_800F05EC(Obj800EFC70 *self) {
    s32 changed;
    func_800EFF0C(self);
    self->vtable_24->update_9C((u8 *)self + self->vtable_24->adjustment_98);
    if (func_800E1CC4(self, 2)) {
        D_8013960C <<= 1;
        self->vtable_24->effect_94((u8 *)self + self->vtable_24->adjustment_90, 0, 2, 255, 0);
        D_8013960C >>= 1;
    }
    changed = func_800E4454(self);
    changed ^= 1;
    if (changed) func_800A7C1C(self);
}

#include "common.h"
typedef unsigned char u8;
typedef struct { u8 pad_00[0xA]; u8 kind_0A; u8 pad_0B[0x13]; u8 flags_1E; u8 pad_1F[0x13]; u8 level_32; u8 pad_33[0x45]; u32 experience_78; } Obj800E9A68;
extern unsigned short func_800E08B0(void *obj);
extern s32 func_800E0F40(void *obj);
extern s32 func_800E9C58(void *obj, u32 experience);
extern void func_800E946C(void *obj, short count);
extern s32 func_80049CB4(s32 id, ...);
extern char *func_800A3B20(void *obj);
extern void func_800498E4(s32 id, ...);
extern s32 func_800A08D8(s32 mode, s32 key, s32 sel);
void func_800E9A68(Obj800E9A68 *self, s32 delta) {
    s32 blocked = 0;
    s32 old_level;
    s32 new_level;
    if (!func_800E08B0(self) || self->kind_0A == 0x1C) blocked = 1;
    if (!blocked) {
        if (self->experience_78 >= 9999999U) { self->experience_78 = 9999999U; return; }
        delta += self->experience_78;
        if (delta < 0) delta = 0;
        else if ((u32)delta > 9999999U) delta = 9999999U;
        self->experience_78 = delta;
        old_level = (u8)func_800E0F40(self);
        self->level_32 = (u8)func_800E9C58(self, delta);
        new_level = (u8)func_800E0F40(self);
        if (old_level != new_level) {
            delta = new_level - old_level;
            func_800E946C(self, delta);
            func_80049CB4(0x20, self, delta);
        }
        if (new_level != old_level) {
            delta = 0x26;
            if (new_level >= old_level) delta = 0x25;
            func_800498E4(delta, func_800A3B20(self), new_level);
            if ((self->flags_1E >> 2) & 1) func_800A08D8(1, -1, 0);
        }
    }
}

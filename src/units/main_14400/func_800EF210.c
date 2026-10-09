#include "common.h"
typedef unsigned char u8;
typedef struct { s32 x, y; } Pos;
typedef struct { u8 pad_00[8]; u8 direction_08; u8 pad_09[0x4F]; void *target_58; u8 pad_5C[0x2C]; s32 field_88; } Object;
extern s32 func_800E20CC(void *self);
extern void *func_800A6CC0(void *out, void *self);
extern void *func_800B4928(Pos *pos);
extern u8 func_800A6420(void *self, void *target);
extern void *func_800A65E4(void *out, void *self, void *target);
extern s32 func_800E1CC4(void *self, s32 kind);
extern void func_800A665C(void *self, u8 *direction);
extern void func_800E8FAC(void *self);
s32 func_800EF210(Object *self) {
    Pos pos;
    u8 calculated;
    u8 direction;
    void *target;
    if (func_800E20CC(self)) {
        func_800A6CC0(&pos, self);
        func_800B4928(&pos);
        direction = self->direction_08;
    } else {
        if (!self->field_88) return 0;
        target = self->target_58;
        if (func_800A6420(self, target)) return 0;
        func_800A65E4(&calculated, self, target);
        direction = calculated;
    }
    if (func_800E1CC4(self, 4)) direction = (direction + 4) & 7;
    func_800A665C(self, &direction);
    func_800E8FAC(self);
    return 1;
}

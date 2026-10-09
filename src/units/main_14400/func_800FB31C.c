#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef struct {
    u8 pad_00[0x1E];
    u8 field_1E;
    u8 pad_1F[0x7B];
    u16 field_9A;
    u8 pad_9C;
    u8 field_9D;
} Actor;
extern s32 func_800E0F40(Actor *obj);
extern s32 func_800F1040(Actor *self, Actor *other, s32 id);
extern s32 func_80049CB4(s32 id, ...);
extern char *func_800A3B20(Actor *obj);
extern void func_800497F0(s32 id, ...);
extern s32 func_800E0F64(Actor *unit, s16 kind, s32 play_extra);
extern void func_800E3678(Actor *obj, Actor *attacker);
extern s32 func_800A08D8(s32 mode, s32 key, s32 sel);


static inline s32 is_blocked(Actor *other) {
    s32 blocked = 0;
    if ((u8)func_800E0F40(other) == 1 ||
        (((other->field_1E >> 4) & 1) && (other->field_9A & 0x40) && other->field_9D == 0)) {
        blocked = 1;
    }
    return blocked;
}

s32 func_800FB31C(Actor *self, Actor *other) {
    s32 state;
    s32 handle;
    if (other != 0 && (other->field_1E & 0x7C)) {
        if (is_blocked(other)) {
            return 0;
        }
    }
    state = func_800F1040(self, other, 0x4D);
    switch (state) {
    case 1:
        return 0;
    case 2:
        return 1;
    }
    func_80049CB4(0x1131);
    handle = func_80049CB4(0x4D, self);
    func_80049CB4(6);
    func_800497F0(0x124, handle, func_800A3B20(self));
    func_80049CB4(7);
    func_80049CB4(0x1131);
    func_80049CB4(6);
    if (func_800E0F64(other, -1, 1)) {
        func_80049CB4(7);
        func_80049CB4(0x23, other, 0, 0x8000);
        func_800E3678(other, self);
        func_800A08D8(1, handle, 0);
    } else {
        func_80049CB4(7);
        func_800E3678(other, self);
    }
    return 1;
}

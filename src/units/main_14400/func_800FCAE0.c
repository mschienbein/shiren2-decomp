#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

/* Entity vtable entry; slots +0x6C and +0x74 are u32 (*)(void *self). */
typedef struct { short delta; short index; u32 (*fn)(void *); } VEntry;
typedef struct { u32 field_0 : 29; u32 special : 1; u32 field_1E : 2; } Flags;
typedef struct Object {
    u8 pad0[0x1E];
    u8 flags_1E;
    u8 pad1F;
    Flags flags_20;
    VEntry *vtable;
    u8 pad28[0x62];
    u8 level_8A;
} Object;

s32 func_800F1040(Object *u, Object *o, s32 id);
s32 func_80049CB4(s32 id, ...);
char *func_800A3B20(Object *u);
void func_800497F0(s32 id, ...);
void func_800E3678(Object *object, Object *attacker);
s32 func_800E0F40(Object *obj);
s32 func_800E0AB4(Object *obj, s32 amount);
short func_800E0BD0(Object *self, s32 amount);
void func_800E0CF4(Object *p, unsigned short percent);
void func_800E20F0(Object *obj);
s32 func_800A08D8(s32 mode, s32 key, s32 sel);

static inline s32 special_flag(Flags *flags) { return flags->special; }
static inline u32 query(Object *self, s32 slot) {
    return self->vtable[slot].fn((u8 *)self + self->vtable[slot].delta);
}
static inline void announce(Object *self, Object *target, s32 message) {
    func_80049CB4(6);
    func_800497F0(0x129, message, func_800A3B20(self));
    func_80049CB4(6);
    func_80049CB4(0x23, target, 0, 0x8000);
    func_800E3678(target, self);
}

/* Unit vtable slot +0xB4 (D_80159440 family): s32 (*)(void *self, void *target). */
s32 func_800FCAE0(void *selfArg, void *targetArg) {
    Object *self = selfArg;
    Object *target = targetArg;
    s32 state = func_800F1040(self, target, 0x50);
    s32 message;
    Flags flags;
    switch (state) {
    case 2: return 1;
    case 1: return 0;
    default: break;
    }
    message = func_80049CB4(0x50, self);
    flags = target->flags_20;
    if (special_flag(&flags)) {
        func_800497F0(0x129, message, func_800A3B20(self));
        func_800E3678(target, self);
        func_800497F0(0x224, message);
    } else if (target->flags_1E & 0xC) {
        if ((u8)func_800E0F40(self) == 1) {
            if ((u16)query(target, 13) < 2) goto unchanged;
            announce(self, target, message);
            func_800E0AB4(target, -1);
        } else {
            /* Slot +0x74 count before and after the drain. */
            u32 count = target->vtable[14].fn((u8 *)target + target->vtable[14].delta);
            u16 difference;
            func_800E0BD0(target, -(u8)func_800E0F40(self) + 1);
            count -= target->vtable[14].fn((u8 *)target + target->vtable[14].delta);
            difference = count;
            if (!difference) goto unchanged;
            announce(self, target, message);
            func_800497F0(0x20, message, difference);
        }
    } else {
        s32 code;
        func_800E20F0(target);
        if ((u16)query(target, 13) < 2) goto unchanged;
        func_800E0CF4(target, 100 - self->level_8A);
        announce(self, target, message);
        code = (u8)func_800E0F40(self) + 0x129;
        func_800497F0(code, message, func_800A3B20(target));
    }
    func_800A08D8(1, message, 0);
    return 1;
unchanged:
    func_800E3678(target, self);
    return 1;
}

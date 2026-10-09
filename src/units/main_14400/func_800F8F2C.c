#include "common.h"
typedef unsigned char u8;
typedef unsigned short u16;
typedef struct { u8 pad0[0x58]; void *target_58; u8 pad5C[0x3E]; u16 flags_9A; } Unit;

s32 func_800E0F40(void *obj);
u8 func_800A6420(void *obj, void *target);
s32 func_800F069C(void *obj);
u32 func_800B1C6C(void *pos);
s32 func_800A67DC(void *unit, void *target, s32 force, s32 apply);
s32 func_800F1024(void *unit);
void *func_800F18A4(void *obj, u8 level, s32 arg);
void func_800F06E4(void *obj);
s32 func_800E1CD4(void *obj, s32 value);
s32 func_800E7104(void *unit);
s32 func_800E8350(void *unit);

static inline s32 has_flag(Unit *self) {
    s32 apply = 0;
    if (self->flags_9A & 0x40) apply = 1;
    return apply;
}

s32 func_800F8F2C(Unit *self) {
    void *target = self->target_58;
    s32 act;
    u8 state = func_800E0F40(self);
    u8 relation = func_800A6420(self, target);

    act = 0;
    if (relation == 0
        || (state == 3 && func_800F069C(self))
        || (state >= 2 && relation != 3 && !(func_800B1C6C(target) & 0x4000))
        || (state == 1 && relation != 3
            && func_800A67DC(self, target, 0, has_flag(self)))) {
        act = 1;
    }
    if (act && func_800F1024(self)) {
        if (target == 0) {
            target = func_800F18A4(self, 0x4C, 0);
            self->target_58 = target;
        }
        func_800F06E4(self);
        return 0;
    }
    if (func_800E1CD4(self, 0x10)) return func_800E8350(self);
    return func_800E7104(self);
}

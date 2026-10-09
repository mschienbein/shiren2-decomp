#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef struct { u32 pad0 : 23; u32 flag_8 : 1; u32 pad1 : 8; } Flags;
typedef struct {
    u8 pad_00[0x90];
    short adjust_90;
    short pad_92;
    s32 (*method_94)(void *, s32, s32, u8, s32);
} VTable;
typedef struct Object {
    u8 pad_00[0x1E];
    u8 flags_1E;
    u8 pad_1F;
    Flags flags_20;
    VTable *vtable_24;
    u8 pad_28[4];
    u16 hp_2C;
} Object;

extern s32 func_800F1040(Object *u, Object *o, s32 id);
extern s32 func_80049CB4(s32 id, ...);
extern char *func_800A3B20(Object *u);
extern void func_800497F0(s32 id, ...);
extern s32 func_800E0F40(Object *obj);
extern void func_800E3678(Object *object, Object *attacker);
extern s32 func_800E0AB4(Object *obj, s32 amount);
extern void func_800E0CF4(Object *p, u16 percent);
extern u32 func_800E110C(const Object *arg0);
extern s32 func_800E1CD4(Object *obj, s32 value);
extern s32 func_800A08D8(s32 mode, s32 key, s32 sel);

static inline s32 has_flag_8(const Flags *flags)
{
    return flags->flag_8;
}

s32 func_801068DC(Object *actor, Object *target)
{
    Flags flags;
    s32 token;
    s32 apply;
    s32 result;

    result = func_800F1040(actor, target, 0x53);
    switch (result) {
    case 2:
        return 1;
    case 1:
        return 0;
    }
    func_80049CB4(0x1131);
    token = func_80049CB4(0x1053, actor);
    func_800497F0(0x14F, token, func_800A3B20(actor));
    switch ((u8)func_800E0F40(actor)) {
    case 1:
        flags = target->flags_20;
        if (has_flag_8(&flags)) {
            func_800E3678(target, actor);
            func_80049CB4(6);
            func_800497F0(0x224, token);
            func_80049CB4(7);
            func_800A08D8(1, token, 0);
        } else {
            if (target->hp_2C < 2) {
                func_800E3678(target, actor);
                break;
            }
            func_80049CB4(0x23, target, 0, 0x8000);
            func_800E3678(target, actor);
            if (target->flags_1E & 0xC) {
                func_800E0AB4(target, -1);
            } else {
                func_800E0CF4(target, 50);
                func_800497F0(0x12A, token, func_800A3B20(target));
            }
            func_800A08D8(1, token, 0);
        }
        break;
    case 2:
        apply = !target->vtable_24->method_94((u8 *)target + target->vtable_24->adjust_90, 2, 0x11, 0, 0)
            || (signed char)func_800E110C(target);
        if (apply) {
            func_80049CB4(0x1131);
            func_80049CB4(6);
            func_80049CB4(0x23, target, 0, 0x8000);
            func_80049CB4(7);
            target->vtable_24->method_94((u8 *)target + target->vtable_24->adjust_90, 0, 0x11, 0xFE, -1);
            func_800A08D8(1, token, 0);
        } else {
            func_800E3678(target, actor);
        }
        break;
    case 3:
        if ((func_800E1CD4(target, 14) ^ 1) != 0) {
            func_80049CB4(0x23, target, 0, 0x8000);
            target->vtable_24->method_94((u8 *)target + target->vtable_24->adjust_90, 0, 14, 0xFE, 0);
            func_800A08D8(1, token, 0);
        }
        break;
    }
    return 1;
}

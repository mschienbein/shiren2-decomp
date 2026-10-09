#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef struct { u32 high:26; u32 immune:1; u32 low:5; } Flags;
typedef struct { u8 pad00[0x1E]; u8 flags1E; u8 pad1F; Flags flags20; u8 pad24[0x65]; u8 amount89; u8 amount8A; u8 pad8B[15]; u16 flags9A; } Entity;
typedef struct { void *source; u32 kind; u32 field08; s16 amount; u16 flags; u8 phase; u8 pad11[3]; s32 field14; } Damage;
extern u32 D_8013960C;
extern Entity *D_801476B8;
extern const u16 D_80156A56;
extern s32 func_800F1040(Entity *object, Entity *target, s32 id);
extern s32 func_800E20CC(void *object);
extern s32 func_80049CB4(s32 id, ...);
extern char *func_800A3B20(void *object);
extern void func_800497F0(s32 id, ...);
extern void func_800498E4(s32 id, ...);
extern void func_80049AE8(s32 id, ...);
extern void func_800E3678(Entity *object, Entity *attacker);
extern s32 func_800EB37C(Entity *object);
extern s32 func_800EB3A4(Entity *object);
extern void func_800EB3CC(Entity *object, s16 amount);
extern void func_800EB488(Entity *object, s16 amount);
extern s32 func_800A08D8(s32 mode, s32 key, s32 selection);
extern u16 func_800E08B0(void *object);
extern void func_80136910(Damage *damage, void *source, u32 amount, u32 kind, u32 flags);
extern void func_800A7ADC(Entity *object, Damage *damage);
extern s32 func_800E0F40(void *object);

static inline Flags entity_flags(Entity *entity)
{
    return entity->flags20;
}

static inline s32 is_immune(Flags *flags)
{
    return flags->immune;
}

s32 func_800FE94C(Entity *object, Entity *target)
{
    Damage damage;
    Flags flags;
    s32 result = func_800F1040(object, target, 0x58);
    s32 message;
    s32 previous;
    u16 current;
    s32 text;
    if (result == 1)
        return 0;
    if (result == 2)
        return 1;
    if (func_800E20CC(object)) {
        func_80049CB4(0x58, object);
        if ((target->flags1E >> 4) & 1) {
            s32 value;
            s32 maximum;
            func_80049CB4(6);
            func_80049CB4(0x23, target, 0, 0x8000);
            func_80049CB4(7);
            func_800498E4(0x13F, func_800A3B20(target));
            func_800E3678(target, object);
            target = D_801476B8;
            value = func_800EB37C(target);
            maximum = func_800EB3A4(target);
            if ((u32)(u8)value < (u32)(u8)maximum) {
                D_8013960C *= 2;
                func_800EB3CC(target, object->amount8A);
                D_8013960C >>= 1;
                func_800498E4(0x140, func_800A3B20(target));
                func_800A08D8(1, -1, 0);
            }
        } else {
            func_800498E4(0x13C, func_800A3B20(object));
            goto consume;
        }
    } else if (object->flags9A & 0x40) {
        s32 value;
        s32 message;
        Damage *event;
        message = func_80049CB4(0x58, object);
        func_80049AE8(0x13F, message, func_800A3B20(object));
        func_800497F0(0x141, message, func_800A3B20(target));
        value = (func_800E08B0(target) * D_80156A56) / 100;
        if (value == 0)
            value = 1;
        event = &damage;
        func_80136910(event, object, (s16)value, 5, 0x8008);
        func_800A7ADC(target, event);
    } else {
        if ((((target->flags1E >> 2) & 1) ^ 1) != 0)
            return 0;
        message = func_80049CB4(0x58, object);
        func_80049CB4(6);
        func_80049AE8(0x13C, message, func_800A3B20(object));
        func_80049CB4(7);
        flags = entity_flags(target);
        if (is_immune(&flags)) {
            func_800497F0(0x224, message);
        } else {
            D_8013960C *= 2;
            if ((func_800E0F40(object) & 0xFF) == 4) {
                previous = func_800EB3A4(target) & 0xFF;
                func_800EB488(target, -(s32)object->amount89);
                text = 0x13E;
                current = func_800EB3A4(target) & 0xFF;
            } else {
                previous = func_800EB37C(target) & 0xFF;
                func_800EB3CC(target, -(s32)object->amount89);
                text = 0x13D;
                current = func_800EB37C(target) & 0xFF;
            }
            D_8013960C >>= 1;
            if (previous == current)
                goto consume;
            func_80049CB4(0x23, target, 0, 0x8000);
            func_80049CB4(6);
            func_800497F0(text, message, current);
            func_80049CB4(7);
        }
        func_800E3678(target, object);
        func_800A08D8(1, message, 0);
    }
    return 1;
    consume:
    func_800E3678(target, object);
    return 1;
}

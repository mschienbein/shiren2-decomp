#include "common.h"

typedef unsigned char u8;
typedef struct { s32 x, y; } Pair;
typedef struct { u8 value; } Dir;
typedef struct {
    u8 pad_00[0x10];
    short adjust_10;
    short pad_12;
    s32 (*method_14)(void *);
    u8 pad_18[0x78];
    short adjust_90;
    short pad_92;
    s32 (*method_94)(void *, s32, s32, u8, s32);
} VTable;
typedef struct {
    Pair pos;
    u8 dir_08;
    u8 pad_09[0x15];
    u8 flags_1E;
    u8 pad_1F;
    s32 field_20;
    VTable *vtable_24;
    u8 pad_28[0x3C];
    Pair pos_64;
    u8 pad_6C[0x1D];
    u8 range_89;
    u8 power_8A;
} Actor;

extern s32 func_800F1040(Actor *u, Actor *o, s32 id);
extern s32 func_80049CB4(s32 id, ...);
extern void func_800497F0(s32 id, ...);
extern char *func_800A3B20(Actor *u);
extern void func_800A7204(void *target, void *source, void *direction, s32 range, s32 damage, s32 message_kind, s32 stop_on_terrain, s32 notify);
extern void func_800A7B18(void *target, void *source, s32 amount, s32 kind);
extern void func_800A7BA4(Actor *obj, s32 value);
extern void *func_800A2594(Pair *out, void *arg, Dir cell);
extern u32 func_800B1C6C(Pair *pos);
extern s32 func_800B43BC(Pair *pos, s32 notify, u8 kind);
extern s32 func_800A251C(Pair *x, Pair *y);
extern unsigned short func_800E08B0(Actor *state);
extern s32 func_800E0F40(Actor *obj);

static inline s32 path_blocked(Actor *actor, Actor *target, Pair *next)
{
    s32 blocked = 0;
    VTable *table;

    if (!(func_800B1C6C(next) & 0x4000)) {
        blocked = 1;
    } else {
        table = target->vtable_24;
        if (table->method_14((u8 *)target + table->adjust_10)) {
            blocked = 1;
        } else if (!func_800B43BC(next, 1, actor->dir_08)) {
            blocked = 1;
        }
    }
    return blocked;
}

s32 func_80106D04(Actor *actor, Actor *target)
{
    Pair origin;
    Pair next;
    Dir direction;
    s32 message;
    s32 remaining;
    s32 valid;
    VTable *table;

    if (func_800F1040(actor, target, 0x6D) == 2) {
        return 1;
    }
    message = func_80049CB4(0x6D, actor);
    func_800497F0(0x150, message, func_800A3B20(actor));
    origin.x = target->pos.x;
    origin.y = target->pos.y;
    direction.value = actor->dir_08;
    func_800A7204(target, actor, &direction, 0x63, 0, 2, 1, 0);
    func_80049CB4(6);
    func_800A7B18(target, actor, actor->power_8A, 0x25);
    func_80049CB4(7);
    if ((direction.value ^ 1) & 1) {
        remaining = actor->range_89;
        for (;;) {
            s32 active = remaining--;
            if (active <= 0) {
                break;
            }
            func_800A2594(&next, target, direction);
            if (path_blocked(actor, target, &next)) {
                break;
            }
            func_800A7204(target, actor, &direction, 0x63, 0, 2, 1, 0);
            func_80049CB4(6);
            func_800A7B18(target, actor, 5, 0x25);
            func_80049CB4(7);
        }
    }
    {
        s32 moved = func_800A251C(&origin, &target->pos) ^ 1;
        if (moved) {
            func_800A7BA4(target, 1);
        }
    }
    valid = 0;
    if ((target->flags_1E & 0x7C) && func_800E08B0(target)) {
        valid = (u8)func_800E0F40(actor) >= 2;
    }
    if (valid) {
        table = target->vtable_24;
        table->method_94((u8 *)target + table->adjust_90, 0, 14, 0xFE, 0);
    }
    actor->pos_64 = target->pos;
    return 1;
}

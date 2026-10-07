#include "common.h"

typedef signed char s8;
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;

typedef struct { s32 x; s32 y; } Pt;
typedef struct { s32 type; s32 unk4[7]; } Msg;
/*
 * Unit vtable (partial): slot 0x90 targets are func_800F212C and its override
 * func_800FF544 (u8 fourth argument); slot 0x98 returns the unit's
 * item list (passed to the iterator constructors by func_800FE5A0/func_800FE3FC).
 */
typedef struct {
    u8 pad0[0x90];
    s16 delta_90;
    s16 index_92;
    s32 (*func_94)(void *self, s32 arg1, s32 arg2, u8 arg3, s32 arg4);
    s16 delta_98;
    s16 index_9A;
    void *(*items_9C)(void *self);
} ActorVTable;
/* Item vtable (partial): slot 0x38 handles a message and reports whether it applied. */
typedef struct {
    u8 pad0[0x38];
    s16 delta_38;
    s16 index_3A;
    s32 (*message_3C)(void *self, Msg *msg);
} ItemVTable;
typedef struct {
    Pt pos;
    u8 pad8[0x1E - 8];
    u8 unk1E;
    u8 pad1F[0x24 - 0x1F];
    ActorVTable *vtable;
    u8 pad28[0x89 - 0x28];
    u8 unk89;
} Actor;
typedef struct {
    u8 unk0;
    u8 pad1[7];
    ItemVTable *vtable;
    u8 padC[3];
    s8 unkF;
} Item;
extern u8 D_80147620[];
s32 func_80049CB4(s32, ...);
void func_800497F0(s32, ...);
s32 func_800E0F40(Actor *);
s32 func_800F1040(Actor *, Actor *, s32);
char *func_800A3B20(void *);
void func_800E20F0(Actor *);
u16 func_800E0ED0(Actor *);
void func_800E0F0C(Actor *, u16);
s32 func_800E1CC4(Actor *, s32);
void *func_800E8A68(Actor *, u8);
Item *func_800FF77C(Actor *, Actor *);
s32 func_8010BEC4(void *, u8);
void func_8010BD3C(Item *, s32);
char *func_800AE674(void *);
s32 func_8010BC2C(void *, u8);
s32 func_800A2D90(u8, u8);
u8 func_800C57CC(u8 *, u8);
void func_8010BE60(Item *, u8);
void func_800E3678(Actor *, Actor *);
s32 func_800A08D8(s32, s32, s32);

static __inline__ void copyPt(Pt *dst, Pt *src) {
    dst->x = src->x;
    dst->y = src->y;
}

static __inline__ s32 isFilled(Item *item, u8 slot) {
    s32 type = item->unk0;
    return func_800A2D90(type, (u8)func_8010BC2C(item, slot)) == 1;
}

static __inline__ s32 canPickUp(Actor *target, u8 flags) {
    s32 ok = 0;
    if (flags & 0xC) {
        ok = target->vtable->items_9C((u8 *)target + target->vtable->delta_98) != 0;
    }
    return ok;
}
s32 func_800FF828(Actor *self, Actor *target) {
    Pt pos;
    Msg msg;
    s32 sound;
    s32 result;
    s32 ok;
    s32 blocked;
    u8 flags;
    u16 amount;
    Item *item;
    s32 count;
    s32 misses;
    s32 pick;
    u8 slot;
    s32 filled;

    if ((u8)func_800E0F40(self) == 1) {
        return 0;
    }
    result = func_800F1040(self, target, 0x4F);
    if (result != 1) {
        if (result == 2) {
            return 1;
        }
    } else {
        return 0;
    }
    copyPt(&pos, &target->pos);
    sound = func_80049CB4(0x4F, self);
    func_800497F0(0x137, sound, func_800A3B20(self));
    func_80049CB4(6);
    func_80049CB4(0xE8, &pos);
    func_80049CB4(7);
    flags = target->unk1E;
    if (flags & 0x30) {
        amount = 0;
        func_800E20F0(target);
        switch ((u8)func_800E0F40(self)) {
        case 2:
            amount = 100 - self->unk89;
            /* fallthrough */
        case 3:
            if (func_800E0ED0(target) == 0) {
                goto fail;
            }
            func_800E0F0C(target, amount);
            func_800497F0((u8)func_800E0F40(self) + 0x136, sound, func_800A3B20(target));
            return 1;
        default:
            blocked = func_800E1CC4(target, 2) == 1;
            if (blocked) {
                goto fail;
            }
            target->vtable->func_94((u8 *)target + target->vtable->delta_90, 0, 2, 0xFE, 0);
            return 1;
        }
    }
    if (!canPickUp(target, flags)) {
        goto fail;
    }
    switch ((u8)func_800E0F40(self)) {
    case 2:
        item = func_800E8A68(target, 3);
        break;
    case 3:
        item = func_800FF77C(self, target);
        break;
    default:
        item = func_800FF77C(self, target);
        if (item == 0) {
            goto fail;
        }
        if ((u8)func_8010BEC4(item, 0x1D)) {
            func_8010BD3C(item, 0x1D);
            func_800497F0(0x13A, sound, func_800AE674(item));
            break;
        }
        count = item->unkF;
        misses = 0;
        if (count == 0) {
            break;
        }
        while (1) {
            if (count-- <= 0) {
                break;
            }
            filled = isFilled(item, count);
            if (!filled) {
                misses++;
            }
        }
        if (misses == 0) {
            break;
        }
        pick = func_800C57CC(D_80147620, misses - 1);
        misses = count = 0;
        while (1) {
            slot = count;
            filled = isFilled(item, slot);
            if (!filled) {
                if (pick == misses) {
                    func_8010BE60(item, slot);
                    func_800497F0(0x13B, sound, func_800AE674(item));
                    return 1;
                }
                misses++;
            }
            count++;
        }
    }
    if (item == 0) {
        goto fail;
    }
    msg.type = 0x18;
    if (item->vtable->message_3C((u8 *)item + item->vtable->delta_38, &msg)) {
        func_80049CB4(0x23, target, 0, 0x8000);
        func_800E3678(target, self);
        func_800A08D8(1, sound, 0);
        return 1;
    }
    func_800E3678(target, self);
    return 1;

fail:
    func_800E3678(target, self);
    func_800497F0(0x224, sound);
    return 1;
}

#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;

typedef struct Object Object;

typedef struct {
    u8 pad0[5];
    u8 bonus_05;
} Stats800E8E60;

typedef struct {
    u8 pad0[0xA0];
    s16 delta_A0;
    s16 padA2;
    Stats800E8E60 *(*stats_A4)(void *self, u8 level);
} VTable800E8E60;

typedef struct {
    u32 pad : 6;
    u32 guarded : 1;
    u32 rest : 25;
} Flags800E8E60;

typedef struct Actor {
    u8 pad0[0xA];
    u8 kind_0A;
    u8 padB[0x15];
    Flags800E8E60 flags_20;
    VTable800E8E60 *vtable_24;
} Actor;

typedef struct {
    Actor *field_00;
    s32 field_04;
    u8 pad_08[4];
    u16 amount_0C;
    u16 field_0E;
} Hit;

u16 func_800E0ED0(Actor *s);
u32 func_800E8D90(Actor *unit);
s32 func_800E0F40(Actor *obj);
s16 func_800A0014(s16 x, u16 y);
void *func_800E8A68(Actor *obj, u8 arg1);
short func_8010D040(Object *object, short amount, s32 flags);

static inline s32 flags_guarded(Flags800E8E60 *flags)
{
    return flags->guarded;
}

s32 func_800E8E60(Actor *obj, Hit *hit)
{
    s16 amount = hit->amount_0C;
    Flags800E8E60 flags;
    Object *item;

    if (!(hit->field_0E & 8)) {
        u16 base = func_800E0ED0(obj);
        s16 bonus = (s16)func_800E8D90(obj);
        Stats800E8E60 *stats = obj->vtable_24->stats_A4((u8 *)obj + obj->vtable_24->delta_A0, func_800E0F40(obj));

        amount = func_800A0014(amount, stats->bonus_05 + (base + bonus / 2));
    }
    flags = obj->flags_20;
    if (flags_guarded(&flags) && (hit->field_0E & 0x800)) {
        return 0;
    }
    if (obj->kind_0A == 0x19 && (hit->field_0E & 0x1000)) {
        return -amount;
    }
    item = func_800E8A68(obj, 4);
    if (item != 0) {
        amount = func_8010D040(item, amount, hit->field_0E);
    }
    return amount != 0 ? amount : 1;
}

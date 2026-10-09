#include "common.h"
typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
/* +8 table: destructor +0xC with delta +0x8. */
typedef struct { s32 unk0, unk4; s16 unk8, unkA; void (*unkC)(void *self, s32 flags); } Dispatch;
typedef struct { s32 unk0, unk4; Dispatch *unk8; } Item;
typedef struct { char pad0[0x1E]; u8 flags_1E; } Target;
typedef struct { char pad0[0xA0]; s32 unkA0; s32 unkA4; } Actor;
typedef struct { s32 x, y; } Position;
typedef struct { void *source; u32 kind; u32 field_8; u16 amount; u16 flags; u8 field_10; } Damage;

void *func_800A6CC0(void *out_position, void *obj);
s32 func_801029B0(void *object, void *position);
s32 func_800E20CC(void *self);
s32 func_800A4520(void *ctx, void *obj);
s32 func_80049CB4(s32 id, ...);
void func_80136910(Damage *damage, void *source, u32 amount, u32 kind, u32 flags);
void func_800A7B68(void *entity, Damage *damage);
void *func_800B4D80(Position *position);
void func_800AD868(Position *position);

static inline Damage *no_damage(Damage *damage) {
    func_80136910(damage, 0, 0, 0, 0);
    return damage;
}

s32 func_80102CAC(Actor *actor, Target *target) {
    Position pos;
    Damage damage;
    Item *item;
    s32 blocked;
    func_800A6CC0(&pos, actor);
    blocked = func_801029B0(actor, &pos) != 1;
    if (blocked) return 0;
    if (target != 0 && (target->flags_1E & 0x7C)) {
        blocked = 0;
        if (func_800E20CC(actor) != 0 || func_800A4520(actor, target) != 0) blocked = 1;
        return blocked == 0;
    }
    func_80049CB4(0x69, actor);
    if (target != 0) {
        func_800A7B68(target, no_damage(&damage));
    } else {
        func_80049CB4(6);
        func_80049CB4(0x109, &pos);
        func_80049CB4(7);
        func_80049CB4(0xE5, &pos);
        item = func_800B4D80(&pos);
        func_800AD868(&pos);
        if (item != 0) item->unk8->unkC((char *)item + item->unk8->unk8, 3);
    }
    actor->unkA0 = 0;
    actor->unkA4 = 0;
    return 1;
}

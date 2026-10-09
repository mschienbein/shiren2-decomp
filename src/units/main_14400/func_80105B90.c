#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef struct Position { s32 x, y; } Position;
typedef struct Dir { u8 value; } Dir;
typedef struct VTable {
    u8 pad_00[0x10];
    short this_delta_10, slot_12;
    s32 (*special_14)(void *self);
    u8 pad_18[0x50];
    short this_delta_68, slot_6A;
    u32 (*power_6C)(void *self);
} VTable;
typedef struct Unit {
    Position position;
    Dir direction_08;
    u8 state_09;
    u8 pad_0A[0x1A];
    VTable *vtable_24;
} Unit;
typedef struct Damage {
    void *source;
    u32 kind_04, flags_08;
    u16 amount_0C, extra_0E;
    u8 duration_10;
    u8 pad_11[7];
} Damage;

/* Whole 12-byte selection record; the mode is the byte at +8. */
typedef struct { unsigned char kind, variant, field_02, flags, row, field_05, field_06, field_07, mode; signed char coordinates[2], status; } SelectionRecord;
extern SelectionRecord D_80142F18;
extern void *func_800A6CC0(void *out_position, void *obj);
extern u32 func_800B1C6C(Position *position);
extern s32 func_800A4314(Unit *object, Position *position);
extern s32 func_80049CB4(s32 id, ...);
extern void func_800A58FC(void *actor, Position *position);
extern void func_800A59A4(Unit *actor);
extern char *func_800A3B20(Unit *actor);
extern s32 func_800A5440(Unit *object, Unit *target, u8 *direction, s32 kind, char *item);
extern void *func_800A2594(Position *out, void *arg, Dir cell);
extern s32 func_800A5D2C(void *object, Position *output, s32 flags);
extern void func_80136910(Damage *damage, void *source, u32 amount, u32 kind, u32 extra);
extern void func_800A7ADC(Unit *entity, Damage *damage);
extern void *func_800A27A4(void *out_direction, void *from, void *to);
extern void func_800A665C(Unit *actor, u8 *direction);
extern void func_800A7C1C(Unit *actor);
extern s32 func_800E20CC(void *actor);
extern void *func_800A6538(void *out_direction, void *obj, void *target);
extern void func_800A6690(Unit *unit, u8 *direction, s32 mode);

/* Damage the target with twice the attacker's power (event brackets 6/7). */
static __inline__ void hit(Damage *damage, void *attacker, Unit *target, u32 power) {
    func_80136910(damage, attacker, (short)(power * 2), 2, 0);
    func_80049CB4(6);
    func_800A7ADC(target, damage);
    func_80049CB4(7);
}
static __inline__ u32 tile_flags(Position *position) { return func_800B1C6C(position); }

/* Unit vtable D_80159440-family +0xB4: s32 (void *self, void *target); self is the acting unit. */
s32 func_80105B90(void *self, void *other)
{
    Unit *target = other;
    Position origin, target_position, destination, computed;
    Damage damage;
    Dir direction, toward, opposite, target_facing, actor_facing;
    s32 result;
    Position *initial = &origin;

    initial->x = ((Unit *)self)->position.x;
    initial->y = ((Unit *)self)->position.y;
    if ((D_80142F18.mode ^ 0x4F) == 0) {
        s32 can_move;
        func_800A6CC0(&target_position, self);
        can_move = 0;
        if (tile_flags(initial) & 0x2000)
            can_move = func_800A4314(((Unit *)self), &target_position) != 0;
        if (can_move) {
            func_80049CB4(0x1097, self, initial, &target_position);
            if (tile_flags(&target_position) & 0x2000)
                func_80049CB4(0x10B, &target_position);
            func_800A58FC(self, &target_position);
            return 1;
        }
        return 0;
    }
    {
        s32 skip = 0;
        if (!(tile_flags(initial) & 0x2000) || !target ||
            target->vtable_24->special_14((u8 *)target + target->vtable_24->this_delta_10) ||
            (target->state_09 & 15) == 1)
            skip = 1;
        if (skip)
            return 0;
    }
    target_position.x = target->position.x;
    target_position.y = target->position.y;
    if (tile_flags(&target_position) & 0xC000)
        return 1;
    func_800A59A4(((Unit *)self));
    func_80049CB4(0x97, self, &origin, &target_position);
    direction = ((Unit *)self)->direction_08;
    func_80049CB4(6);
    result = func_800A5440(target, ((Unit *)self), &direction.value, 0, func_800A3B20(((Unit *)self)));
    func_80049CB4(7);
    if (result == 2) {
        destination = ((Unit *)self)->position;
    } else {
        s32 blocked;
        func_800A2594(&computed, &target_position, direction);
        destination = computed;
        blocked = func_800A4314(((Unit *)self), &destination) != 1;
        if (blocked) {
            destination = target_position;
            func_800A5D2C(self, &destination, 1);
        }
    }
    if (result == 2) {
        ((Unit *)self)->position = target_position;
        func_80049CB4(0x88, self);
        func_80049CB4(0x8C, self, &target_position, &destination);
    } else {
        if (!result) {
            u32 power = ((Unit *)self)->vtable_24->power_6C((u8 *)self + ((Unit *)self)->vtable_24->this_delta_68);
            hit(&damage, self, target, power);
        }
        computed.x = target_position.x;
        computed.y = target_position.y;
        func_800A27A4(&toward, &destination, &computed);
        direction = toward;
        func_800A2594(&computed, &target_position, direction);
        ((Unit *)self)->position = computed;
        opposite.value = (direction.value + 4) & 7;
        func_800A665C(((Unit *)self), &opposite.value);
        func_80049CB4((tile_flags(&destination) & 0x2000) ? 0x1098 : 0x1099, self);
    }
    if (tile_flags(&destination) & 0x2000)
        func_80049CB4(0x10B, &destination);
    func_800A58FC(self, &destination);
    func_800A7C1C(((Unit *)self));
    if (func_800E20CC(self))
        func_80049CB4(0xA9, self);
    func_800A6538(&target_facing, target, &destination);
    func_800A6690(target, &target_facing.value, 1);
    func_800A6538(&actor_facing, self, &target_position);
    func_800A6690(((Unit *)self), &actor_facing.value, 1);
    return 1;
}

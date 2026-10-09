#include "common.h"
typedef unsigned char u8;
typedef struct { s32 x, y; } Position;
typedef struct { u8 pad_00[3]; u8 state_03; } Item;
typedef struct { u8 pad_00[0x20]; short count_delta_20, reserved_22; s32 (*count_24)(void *); u8 pad_28[0x10]; short at_delta_38, reserved_3A; void *(*at_3C)(void *, u32); } InventoryVTable;
typedef struct { s32 field_00; InventoryVTable *vtable_04; } Inventory;
typedef struct { u32 pad0 : 21; u32 bit10 : 1; u32 pad1 : 10; } Flags;
typedef struct { Position position_00; u8 direction_08; u8 pad_09[0x15]; u8 flags_1E; u8 field_1F; Flags flags_20; u8 pad_24[0x40]; void *target_64; void *target_68; u8 pad_6C[0x20]; Inventory *inventory_8C; } Entity;
typedef struct { unsigned char kind, variant, field_02, flags, row, field_05, field_06, field_07, mode; signed char coordinates[2], status; } SelectionRecord;
extern SelectionRecord D_80142F18;
s32 func_80049CB4(s32 id, ...);
void *func_800A6CC0(void *out_position, void *object);
s32 func_800E20CC(void *object);
s32 func_800B4888(void *position);
void func_800498E4(s32 id, ...);
s32 func_800AD714(Item *item, Position *position);
u32 func_800B1C6C(Position *position);
char *func_800AE674(void *item);
s32 func_800AD8AC(Item *item, Position *position);
void *func_800F1750(void *entity);
s32 func_800A4754(void *entity, void *position, u8 *direction);
s32 func_800A455C(void *entity, void *other, s32 range);
s32 func_800FA1FC(Entity *entity, Entity *other);
s32 func_800A4520(void *entity, void *other);
Item *func_800FA298(Entity *entity, Entity *other);
void *func_800B4D80(Position *position);
s32 func_800FA710(Entity *entity, Position *position);
s32 func_800A674C(void *entity, void *other);
s32 func_800F069C(void *entity);
void func_800497F0(s32 id, ...);
void func_800E3678(void *other, void *entity);
void func_800AD868(Position *position);
char *func_800A3B20(void *entity);
s32 func_800CD5C0(void *inventory, Item *item);
s32 func_800A529C(void *entity, s32 flag);
static inline Position *copy_position(Position *out, Position *source)
{
    out->x = source->x;
    out->y = source->y;
    return out;
}
static inline void copy_flags(Flags *out, Flags *source)
{
    *out = *source;
}
s32 func_800FAE58(Entity *entity, Entity *other)
{
    Position current, destination;
    Item *item;
    Flags flags;
    s32 blocked, from_floor, guarded;
    s32 invalid_destination;
    u8 direction;
    s32 home = D_80142F18.mode == 0x4F;
    if (home) {
        func_80049CB4(0x1056, entity);
        return 1;
    }
    copy_position(&current, &entity->position_00);
    func_800A6CC0(&destination, entity);
    if (func_800E20CC(entity)) {
        Inventory *inventory = entity->inventory_8C;
        item = inventory->vtable_04->at_3C((u8 *)inventory + inventory->vtable_04->at_delta_38, 0);
        if (item) {
            Position *next = &destination;
            Position *from;
            func_80049CB4(0x1131);
            func_80049CB4(6);
            func_80049CB4(0x56, entity);
            func_80049CB4(7);
            if (func_800B4888(next)) {
                func_800498E4(0x123);
                return 1;
            }
            invalid_destination = 0;
            if (!func_800AD714(item, next)
                || ((func_800B1C6C(&current) & 0x2000) && !(func_800B1C6C(next) & 0x2000))) {
                invalid_destination = 1;
            }
            if (invalid_destination) {
                func_800498E4(0x7B);
                return 1;
            }
            from = &current;
            if (func_800B1C6C(from) & 0x2000) {
                item->state_03 = 1;
                func_80049CB4(0xD1, item, from, &destination);
            } else {
                func_80049CB4(0xB9, item, from, &destination);
            }
            func_800498E4(0x7C, func_800AE674(item));
            func_800AD8AC(item, &destination);
            func_800F1750(entity);
            return 1;
        }
    }
    blocked = 0;
    if (other && (other->flags_1E & 3)) {
        blocked = 1;
    } else {
        Inventory *inventory = entity->inventory_8C;
        if (inventory->vtable_04->count_24((u8 *)inventory + inventory->vtable_04->count_delta_20)) {
            blocked = 1;
        }
    }
    if (blocked) {
        return 0;
    }
    item = 0;
    from_floor = 0;
    direction = entity->direction_08;
    if (func_800A4754(entity, &current, &direction)) {
        if (other && func_800A455C(entity, other, 1)) {
            s32 can_take = 0;
            if (!(guarded = other->flags_20.bit10, copy_flags(&flags, &other->flags_20), guarded) && func_800FA1FC(entity, other)) {
                can_take = func_800A4520(entity, other) != 0;
            }
            if (can_take) {
                item = func_800FA298(entity, other);
            }
        }
        if (!item) {
            item = func_800B4D80(&destination);
            if (func_800FA710(entity, &destination)) {
                from_floor = 1;
            } else {
                item = 0;
            }
        }
    }
    if (!item) {
        if (other && func_800A674C(entity, other)) {
            s32 message = func_80049CB4(0x56, entity);
            func_800497F0(func_800F069C(entity) ? 0x122 : 0x121, message);
            func_800E3678(other, entity);
            return 1;
        }
        if (func_800E20CC(entity)) {
            func_80049CB4(0x56, entity);
        }
        return 1;
    } else {
        s32 message;
        char *name;
        Position *from;
        func_80049CB4(0x56, entity);
        from = &current;
        if (func_800B1C6C(from) & 0x2000) {
            message = func_80049CB4(0xD1, item, &destination, from);
        } else {
            message = func_80049CB4(0xB9, item, &destination, from);
        }
        if (from_floor) {
            func_800AD868(&destination);
        } else if (other) {
            func_800E3678(other, entity);
        }
        name = func_800A3B20(entity);
        func_800497F0(0x120, message, name, func_800AE674(item));
        func_800CD5C0(entity->inventory_8C, item);
        entity->target_64 = 0;
        entity->target_68 = 0;
        func_80049CB4(0x132);
        func_800A529C(entity, 1);
    }
    return 1;
}

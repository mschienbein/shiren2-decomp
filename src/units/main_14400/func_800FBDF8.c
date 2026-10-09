#include "common.h"
typedef unsigned char u8;
typedef unsigned short u16;
typedef struct { u32 pad0 : 16; u32 reflect : 1; u32 pad1 : 15; } Flags;
/* Collection/item link filled by func_800D0190. */
typedef struct { void *field_0; void *field_4; } Link;
/* Command table (D_80157FA8 hierarchy): +0xC destructor, +0x14 execute. */
typedef struct {
    u8 pad_00[8]; short delta_08; short pad_0A; void (*destroy_0C)(void *self, s32 flags);
    short delta_10; short pad_12; s32 (*execute_14)(void *self);
} CommandTable;
typedef struct { s32 field_00; CommandTable *table_04; } Command;
/* Item message table at +8: +0x1C kind query. */
typedef struct { u8 pad_00[0x18]; short delta_18; short pad_1A; s32 (*is_kind_1C)(void *self, s32 kind); } ItemTable;
typedef struct { u8 kind; u8 id; u8 pad_02[6]; ItemTable *table_08; } Item;
/* Item set table: +0x24 count, +0x3C get(index). */
typedef struct {
    u8 pad_00[0x20]; short delta_20; short pad_22; s32 (*count_24)(void *self);
    u8 pad_28[0x10]; short delta_38; short pad_3A; void *(*get_3C)(void *self, u32 index);
} ItemSetTable;
typedef struct { s32 field_00; ItemSetTable *table_04; } ItemSet;
/* Monster table at +0x24: +0x94 status action, +0x9C item set. */
typedef struct {
    u8 pad_00[0x90]; short delta_90; short pad_92; s32 (*act_94)(void *self, s32 mode, s32 value, u8 byte, s32 extra);
    short delta_98; short pad_9A; void *(*items_9C)(void *self);
} MonsterTable;
typedef struct {
    u8 pad_00[0x1E]; u8 field_1E; u8 pad_1F; Flags flags_20; MonsterTable *table_24;
    u8 pad_28[0x4A]; u8 field_72; u8 pad_73[0x91]; s32 field_104;
} Monster;
extern u8 D_80147620[];
typedef struct Buf800D8FB0 Buf800D8FB0;
s32 func_800F1040(Monster *user, Monster *target, s32 id);
s32 func_80049CB4(s32 id, ...);
void func_80049AE8(s32 message, ...);
void func_800497F0(s32 id, ...);
char *func_800A3B20(void *unit);
void *func_800A65E4(u8 *direction, Monster *object, Monster *target);
void func_800A665C(Monster *obj, u8 *value);
void func_800A6690(Monster *unit, u8 *direction, s32 arg2);
s32 func_800E1CD4(Monster *obj, s32 value);
void func_800E3678(Monster *object, Monster *attacker);
u8 func_800C57A0(void *rng);
u8 func_800C57CC(void *rng, s32 limit);
void *func_800D0190(Link *output, void *helper, void *item);
Buf800D8FB0 *func_800D8FB0(u32 size);
Command *func_800DAE40(Buf800D8FB0 *obj, Link *src);
Command *func_800DB1F8(Buf800D8FB0 *s, Link *src);
Command *func_800DAF10(Buf800D8FB0 *s, Link *src);
Command *func_800DC4C0(Buf800D8FB0 *self, Link *source, Link *entry);
Command *func_800DC3F0(Buf800D8FB0 *arg, Link *src);
Command *func_800DCCC0(Buf800D8FB0 *a, Link *src);
Command *func_800DBD00(Buf800D8FB0 *self, Link *source, Link *entry);
Command *func_800DD6E0(Buf800D8FB0 *state, Link *source);
Command *func_800DBB90(Buf800D8FB0 *a, Link *src);
Command *func_800DD3F0(Buf800D8FB0 *obj, Link *src);
Command *func_800DD320(Buf800D8FB0 *object, Link *src);
Command *func_800DC730(Buf800D8FB0 *self, Link *src);
static inline s32 reflects(Flags *flags) {
    return flags->reflect;
}
static inline s32 can_steal(Monster *target) {
    s32 result = 0;
    if (((target->field_1E >> 2) & 1) && !target->field_104
        && !target->table_24->act_94((u8 *)target + target->table_24->delta_90, 2, 9, 0, 0))
        result = !func_800E1CD4(target, 15);
    return result;
}
static inline void *item_get(ItemSet *items, u32 index) {
    return items->table_04->get_3C((u8 *)items + items->table_04->delta_38, index);
}
static inline s32 item_is(Item *item, s32 kind) {
    return item->table_08->is_kind_1C((u8 *)item + item->table_08->delta_18, kind);
}
s32 func_800FBDF8(Monster *user, Monster *target) {
    Link link;
    Link other;
    Flags flags;
    u8 direction;
    u8 facing;
    s32 msg;
    s32 result;
    ItemSet *items;
    s32 count;
    Item *item;
    Command *command;
    result = func_800F1040(user, target, 0x4E);
    switch (result) {
    case 2:
        return 1;
    case 1:
        return 0;
    }
    func_80049CB4(0x1131);
    func_80049CB4(6);
    msg = func_80049CB4(0x4E, user);
    func_80049CB4(7);
    func_80049AE8(0x127, msg, func_800A3B20(user));
    flags = target->flags_20;
    if (reflects(&flags)) {
        func_800A65E4(&direction, target, user);
        func_800A665C(target, &direction);
        func_80049CB4(0x1065, target);
        func_80049AE8(0x50, msg, func_800A3B20(target));
        target = user;
    } else if (can_steal(target)) {
        items = target->table_24->items_9C((u8 *)target + target->table_24->delta_98);
        count = items->table_04->count_24((u8 *)items + items->table_04->delta_20);
        if (!count || (target->field_72 & 4)) {
            target->field_72 |= 4;
            return 1;
        }
        target->field_72 |= 4;
        item = item_get(items, func_800C57CC(D_80147620, (u8)(count - 1)));
        func_800D0190(&link, items, item);
        command = 0;
        switch (item->kind) {
        case 5:
        case 11:
        case 12:
        case 13:
            if (func_800C57A0(D_80147620) & 1) break;
        case 3:
        case 4:
        case 6:
            command = func_800DB1F8(func_800D8FB0(0x10), &link);
            break;
        case 1:
            command = func_800DAE40(func_800D8FB0(0x10), &link);
            break;
        case 8:
            command = func_800DAF10(func_800D8FB0(0x10), &link);
            break;
        case 2:
            if (!(func_800C57A0(D_80147620) & 7)) break;
            if (item_is(item, 6)) {
                func_800D0190(&other, items, item_get(items, func_800C57CC(D_80147620, (u8)(count - 1))));
                command = func_800DC4C0(func_800D8FB0(0x18), &link, &other);
            } else {
                command = func_800DC3F0(func_800D8FB0(0x10), &link);
            }
            break;
        case 7:
            command = func_800DCCC0(func_800D8FB0(0x10), &link);
            break;
        case 9:
            if (item_is(item, 14)) {
                func_800D0190(&other, items, item_get(items, func_800C57CC(D_80147620, (u8)(count - 1))));
                command = func_800DBD00(func_800D8FB0(0x18), &link, &other);
            } else if (item_is(item, 27)) {
                command = func_800DD6E0(func_800D8FB0(0x10), &link);
            } else if (item_is(item, 18)) {
                command = func_800DBB90(func_800D8FB0(0x10), &link);
            }
            break;
        default:
            if (item->id == 0xEC) command = func_800DD3F0(func_800D8FB0(0x10), &link);
            if (item->id == 0xE9) command = func_800DD320(func_800D8FB0(0x10), &link);
            break;
        }
        if (!command) {
            command = func_800DC730(func_800D8FB0(0x10), &link);
            facing = func_800C57A0(D_80147620) & 7;
            func_800A6690(target, &facing, 1);
        } else {
            func_800E3678(target, user);
        }
        func_80049CB4(0x132);
        func_800497F0(0x128, msg, func_800A3B20(target));
        command->table_04->execute_14((u8 *)command + command->table_04->delta_10);
        if (command) command->table_04->destroy_0C((u8 *)command + command->table_04->delta_08, 3);
        return 1;
    }
    target->table_24->act_94((u8 *)target + target->table_24->delta_90, 0, 0x13, 0xFE, 0);
    return 1;
}

#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef struct Actor {
    u8 pad_00[0x1E];
    u8 field_1E;
} Actor;
typedef struct Item Item;
typedef struct {
    Actor *field_00;
    s32 field_04;
    u8 pad_08[6];
    u16 field_0E;
} Hit;
typedef struct {
    u8 pad_00[0x10];
    Hit *field_10;
} Event;
/* func_800E8E60 returns a full int (its negated path at 0x800E8F4C is not
 * re-narrowed); this caller keeps the int and narrows it to short at each use
 * (sll/sra at 0x800EA9CC, 0x800EAA2C and 0x800EAA5C/0x800EAA74). */
extern s32 func_800E8E60(Actor *obj, Hit *hit);
extern void func_800E3884(Actor *obj, Hit *hit, s32 amount);
extern s32 func_80049CB4(s32 id, ...);
extern u16 func_800E08B0(void *obj);
extern void *func_800E8A68(Actor *obj, u8 kind);
extern void func_8010D178(Item *self, Actor *owner, Actor *target, short damage, s32 kind);
extern u8 func_800E8B10(Actor *obj, Item **out);
extern void func_801136C4(Item *self, Actor *owner, s16 damage, s32 kind, u16 flags);

void func_800EA998(Actor *obj, Event *event) {
    Item *items[2];
    Hit *hit = event->field_10;
    s32 damage = func_800E8E60(obj, hit);
    Item *item;
    s32 i;
    if ((s16)damage != 0) {
        func_800E3884(obj, hit, (s16)damage);
        if ((obj->field_1E >> 2) & 1) {
            func_80049CB4(0x89, obj);
        }
    }
    if (func_800E08B0(obj)) {
        item = func_800E8A68(obj, 4);
        if (item != 0) {
            func_8010D178(item, obj, hit->field_00, (s16)damage, hit->field_04);
        }
        i = func_800E8B10(obj, items);
        while (--i != -1) {
            func_801136C4(items[i], obj, (s16)damage, hit->field_04, hit->field_0E);
        }
    }
}

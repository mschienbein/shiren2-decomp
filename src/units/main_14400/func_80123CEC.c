#include "common.h"
typedef unsigned char u8;
typedef unsigned short u16;
typedef struct { s32 x; s32 y; } Pos;
typedef struct Msg { s32 id; u8 payload_04[0x18]; } Msg;
typedef struct ActorTable {
    u8 pad_00[0x78]; short adjustment_78, pad_7A;
    void (*level_7C)(void *, short);
    u8 pad_80[0x10]; short adjustment_90, pad_92;
    s32 (*effect_94)(void *, s32, s32, u8, s32);
} ActorTable;
typedef struct Actor { u8 pad_00[0xA]; u8 kind_0A; u8 pad_0B[0x13]; u8 flags_1E; u8 pad_1F[5]; const ActorTable *vtable_24; } Actor;
typedef struct ItemTable { u8 pad_00[0x38]; short adjustment_38, pad_3A; s32 (*message_3C)(void *, Msg *); } ItemTable;
typedef struct Item { u8 kind_00; u8 pad_01[7]; const ItemTable *vtable_08; } Item;
extern s32 func_80049CB4(s32 id, ...);
extern s32 func_800E8C64(void *owner);
extern void func_800498E4(s32 id, ...);
extern u16 func_800E0ED0(Actor *self);
extern void func_800E0F0C(Actor *self, u16 value);
extern char *func_800A3B20(Actor *self);
extern s32 func_800E0F40(Actor *self);
/* Slot 0x44 retains unused self, owner, from and direction supplied by 80116028..80116050. */
s32 func_80123CEC(Item *self, Actor *owner, Pos *from, Pos *at, void *direction, Actor *target, Item *item) {
    Msg message;
    s32 kind, changed;
    func_80049CB4(0xE8, at);
    if (target) {
        if (target->flags_1E & 0xC) {
            func_80049CB4(0x41, target);
            changed = func_800E8C64(target);
            changed ^= 1;
            if (changed) func_800498E4(0x223);
        } else if (target->flags_1E & 0x7C) {
            kind = target->kind_0A;
            if (kind == 0x33) {
                target->vtable_24->level_7C((u8 *)target + target->vtable_24->adjustment_78, 1);
            } else {
                if (func_800E0ED0(target)) {
                    func_800E0F0C(target, 0);
                    func_800498E4(0xF8, func_800A3B20(target));
                }
                if (kind == 0x37 && (u8)func_800E0F40(target) >= 2) {
                    target->vtable_24->effect_94((u8 *)target + target->vtable_24->adjustment_90, 0, 0x11, 0xFE, -1);
                }
            }
        }
    }
    if (item && (u32)(item->kind_00 - 3) < 2) {
        message.id = 0x18;
        item->vtable_08->message_3C((u8 *)item + item->vtable_08->adjustment_38, &message);
    }
    return 1;
}

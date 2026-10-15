#include "common.h"
#include "collection_iterator.h"
#include "item_records.h"

typedef unsigned char u8;
typedef struct UnitVTable {
    u8 pad_00[0x90];
    short this_delta_90, slot_92;
    s32 (*condition_94)(void *, s32, s32, u8, s32);
} UnitVTable;
typedef struct Unit {
    u8 pad_00[0x1E];
    u8 flags_1E;
    u8 pad_1F[5];
    UnitVTable *vtable_24;
    u8 pad_28[0xDC];
    s32 state_104;
} Unit;
typedef struct Message {
    s32 kind;
    Unit *actor_04, *target_08;
    u8 pad_0C[0xC];
    s32 enabled_18, quiet_1C;
} Message;
typedef Message Msg;
typedef struct Entry { u8 field_00, kind_01; } Entry;
typedef Entry Ent;
typedef struct Rng Rng;
extern Rng D_80147620;
extern Unit *D_801476B8;
extern s32 D_80147678;
extern u8 D_8013960A;
extern const u8 D_80156A7D, D_80156A7F, D_80156A81, D_80156A83;
extern void func_80120B90(Item *object, Unit *actor);
extern void *func_8011422C(Item *object);
extern void func_800CD3D0(void *list, u32 index);
extern s32 func_80049CB4(s32 kind, ...);
extern s32 func_800E1CD4(Unit *actor, s32 kind);
extern s32 func_800C5844(void *rng, u8 base, u8 top);
extern void func_800EC68C(Unit *actor, u32 value);
extern void func_800498E4(s32 kind, ...);
extern s32 func_8011459C(void *object, void *position);
extern void func_800D3650(void *object);
extern s32 func_800A533C(u8 *object);
extern s32 func_80114E28(Item *object, Msg *message);

static inline s32 message_kind(Message *message)
{
    return message->kind;
}

s32 func_80120D88(Item *object, Message *message)
{
    CollectionIterator iterator;
    switch (message_kind(message)) {
    case 16:
        func_80120B90(object, message->actor_04);
        return 1;
    case 15: {
        s32 enabled = message->enabled_18 != 0;
        if (!enabled && !message->quiet_1C) {
            func_800CEB20(&iterator, func_8011422C(object));
            while (func_800CEBA0(&iterator)) {
                if (((Ent *)func_800CEC68(&iterator))->kind_01 == 0xEF)
                    func_800CD3D0(func_8011422C(object), iterator.index + 1);
            }
            if (object->flags02 & 4)
                func_80049CB4(0x85, message->actor_04, object);
        }
        if (enabled)
            object->flags02 |= 4;
        else
            object->flags02 &= ~4;
        return 1;
    }
    case 18: case 19: {
        s32 skip = 0;
        Unit *target = message->target_08;
        s32 duration, special;
        if (!(target->flags_1E & 0x7C) || func_800E1CD4(target, 15))
            skip = 1;
        if (skip)
            break;
        duration = 0xFF;
        if (object->payload.pot.field28) {
            u8 lower, upper;
            if (target->flags_1E & 0xC) {
                lower = D_80156A7D;
                upper = D_80156A7F;
            } else {
                lower = D_80156A81;
                upper = D_80156A83;
            }
            duration = func_800C5844(&D_80147620, lower, upper);
        }
        target->vtable_24->condition_94((u8 *)target + target->vtable_24->this_delta_90,
                                      0, 15, (u8)duration, 0);
        special = 0;
        if (!object->payload.pot.field28 && ((target->flags_1E >> 2) & 1))
            special = target->state_104 == 0;
        if (special) {
            func_800EC68C(D_801476B8, 29);
            D_80147678 = 1;
            D_8013960A = 0;
            func_800498E4(0xC4);
            func_80049CB4(0x129, 0x32);
        }
        if (message->kind == 18) {
            func_8011459C(object, target);
            func_800D3650(object);
            if (object)
                object->field08->method0C((u8 *)object + object->field08->delta08, 3);
        }
        func_800A533C((u8 *)target);
        return 1;
    }
    }
    return func_80114E28(object, message);
}

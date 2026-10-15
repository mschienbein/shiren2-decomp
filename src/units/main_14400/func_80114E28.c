#include "common.h"
#include "item_records.h"
typedef unsigned char u8;
typedef unsigned short u16;
typedef struct { s32 x, y; } Pos;
typedef struct Actor Actor;
typedef struct { ItemMessage base; Actor *source; void *target; u8 direction; u8 pad_0D[3]; Pos pos; s32 field_18; Actor *owner; } Event;
typedef struct { u8 pad_00[0x10]; short delta_10, index_12; s32 (*status_14)(void *); } ActorTable;
struct Actor { Pos pos; u8 pad_08[0x16]; u8 flags_1E; u8 pad_1F[5]; ActorTable *table_24; };
/* Partial menu-system view: func_80094DA0 clears the word at +0x84. */
typedef struct { u8 pad_00[0x84]; s32 active_84; } MenuState80114E28;
extern MenuState80114E28 D_80140160;
extern u8 D_80147620[];
extern void func_800498E4(s32, ...);
extern s32 func_80049CB4(s32, ...);
extern char *func_80083C90(char *, char *);
extern void func_800A7B18(void *, void *, s32, s32);
extern s32 func_800AF28C(Item *, void *);
extern s32 func_800C5844(void *, u8, u8);
extern s32 func_800CD1FC(void *);
extern s32 func_800CD278(void *);
extern s32 func_800CD2BC(void *, void *);
extern void func_800CD468(void *);
extern s32 func_800CD508(void *, void *);
extern s32 func_800CD5C0(void *, void *);
extern void func_800D3650(void *);
extern void *func_8011422C(void *);
extern void func_80114268(void *, s32);
extern s32 func_80114330(Item *, Item *);
extern void func_801146A4(Item *, void *, u16);
extern void func_80114D50(void *, s32, s32);
extern void func_8011541C(void *);
/* ODD_C: inherited value/status/apply/get helpers preserve whole-object
 * addressing and virtual-call scheduling. Their direct inlining has not
 * been measured by this cleanup; the menu field is accessed directly. */
static inline s32 value(ItemList *list) { return list->methods->limit14((u8 *)list + list->methods->delta10); }
static inline s32 status(Actor *actor) { return actor->table_24->status_14((u8 *)actor + actor->table_24->delta_10); }
/* Kind-9 constructor func_80114060 installs D_8015D938 with an ItemMethods prefix. */
static inline Item *apply(Item *item, Actor *actor, Item *target) { ItemPotMethods *methods = (ItemPotMethods *)item->field08; return methods->apply44((u8 *)item + methods->delta40, actor, target); }
static inline Item *get(ItemList *list, u32 index) { return list->methods->get3C((u8 *)list + list->methods->delta38, index); }
static inline void makeDropEvent(Event *message, Actor *source, Pos *position) {
    message->base.kind = 0xE;
    message->source = source;
    message->pos = *position;
}
s32 func_80114E28(Item *self, Event *event) {
    /* ODD_C: event cases are exclusive and share one original 32-byte scratch slot. */
    union { char name[0x20]; Event message; } scratch;
    void *inventory = func_8011422C(self);
    switch (event->base.kind) {
    case 10: {
        Item *item = event->target;
        Actor *actor = event->source;
        ItemList *list;
        s32 before, free;
        if (!func_80114330(self, item)) return 0;
        list = &self->payload.pot.list;
        if ((actor->flags_1E >> 2) & 1) {
            if (!D_80140160.active_84) {
                char *name = func_800AE674(self);
                func_800498E4(0x93, name, func_800AE674(item));
                func_80049CB4(6);
                func_80049CB4(0x10D3, item);
                func_80049CB4(7);
            } else func_80049CB4(0x10D3, item);
            func_80049CB4(0x3A, actor);
            list = &self->payload.pot.list;
        }
        before = value(list);
        free = func_800CD278(list);
        item = apply(self, actor, item);
        if (item) func_800CD5C0(inventory, item);
        func_80114D50(self, before, free);
        if (D_80140160.active_84) func_80049CB4(0x138);
        return 1;
    }
    case 11: {
        s32 changed = 0;
        Actor *actor = event->source;
        ItemList *from = event->target;
        s32 before, free;
        u32 index = 0;
        func_80083C90(scratch.name, func_800AE674(self));
        before = value(&self->payload.pot.list);
        free = func_800CD278(&self->payload.pot.list);
        while (index < (u32)from->methods->count24((u8 *)from + from->methods->delta20)) {
            Item *item;
            Item *result;
            s32 blocked;
            item = get(from, index);
            blocked = 0;
            if (!from->methods->can_release64((u8 *)from + from->methods->delta60, item, 0) ||
                !func_800CD508(inventory, item) || item->field00 == 9 || item->field00 == 0x11) blocked = 1;
            if (blocked) { index++; continue; }
            if (!changed) func_80049CB4(0x3A, actor);
            func_800CD2BC(from, item);
            result = apply(self, actor, item);
            if (result) { func_80049CB4(0x10D3, result); func_800CD5C0(inventory, result); }
            if (status(actor)) return 1;
            changed++;
        }
        if (changed > 0) { func_80114D50(self, before, free); func_800498E4(0x94, scratch.name, changed); }
        else func_800498E4(0x95, scratch.name);
        func_80049CB4(0x129, 10);
        func_80049CB4(0x138);
        return 1;
    }
    case 18: {
        Actor *target = event->target;
        s32 active = 0;
        if (self->payload.pot.field28 && (!((target->flags_1E >> 2) & 1) || !status(target))) active = 1;
        if (active) {
            Actor *source;
            makeDropEvent(&scratch.message, event->source, &target->pos);
            self->field08->handle3C((u8 *)self + self->field08->delta38, &scratch.message.base);
            source = event->source;
            func_80049CB4(6);
            func_800A7B18(target, source, (u8)func_800C5844(D_80147620, 1, 3), 0x21);
            func_80049CB4(7);
            func_80049CB4(0x132);
            return 1;
        }
        return func_800AF28C(self, event);
    }
    case 14: {
        u16 message = 0xA0;
        if (event->source) message = 0xA1;
        func_801146A4(self, &event->pos, message);
        func_800D3650(self);
        if (self) self->field08->method0C((u8 *)self + self->field08->delta08, 3);
        return 1;
    }
    case 28: {
        s32 before = func_800CD1FC(func_8011422C(self));
        func_800CD468(func_8011422C(self));
        func_80114268(self, -before);
        func_8011541C(self);
        return 1;
    }
    default:
        return func_800AF28C(self, event);
    }
}

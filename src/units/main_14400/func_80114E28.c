#include "common.h"
typedef unsigned char u8;
typedef unsigned short u16;
typedef struct { s32 x, y; } Pos;
typedef struct Item Item;
typedef struct Actor Actor;
typedef struct { s32 kind; Actor *source; void *target; u8 direction; u8 pad_0D[3]; Pos pos; s32 field_18; Actor *owner; } Event;
typedef struct {
    u8 pad_00[0x10]; short delta_10, index_12; s32 (*value_14)(void *);
    u8 pad_18[8]; short delta_20, index_22; s32 (*count_24)(void *);
    u8 pad_28[0x10]; short delta_38, index_3A; void *(*get_3C)(void *, u32);
    u8 pad_40[0x20]; short delta_60, index_62; s32 (*accept_64)(void *, void *, s32);
} CollectionTable;
typedef struct { const void *primary; CollectionTable *table_04; u8 pad_08[0x10]; void *owner_18; } Collection;
typedef struct {
    u8 pad_00[8]; short delta_08, index_0A; void (*destroy_0C)(void *, s32);
    u8 pad_10[0x28]; short delta_38, index_3A; s32 (*event_3C)(void *, Event *);
    short delta_40, index_42; Item *(*apply_44)(void *, Actor *, Item *);
} ItemTable;
struct Item { u8 kind; u8 pad_01[7]; ItemTable *table_08; Collection collection_0C; u8 flags_28; };
typedef struct { u8 pad_00[0x10]; short delta_10, index_12; s32 (*status_14)(void *); } ActorTable;
struct Actor { Pos pos; u8 pad_08[0x16]; u8 flags_1E; u8 pad_1F[5]; ActorTable *table_24; };
extern signed char D_80140160[];
/* ODD_C: menu and virtual-call helpers preserve whole-object addressing and scheduling. */
static inline s32 menu_active(const signed char *menu) { return *(const s32 *)(menu + 0x84); }
extern u8 D_80147620[];
extern void func_800498E4(s32, ...);
extern s32 func_80049CB4(s32, ...);
extern char *func_80083C90(char *, char *);
extern void func_800A7B18(void *, void *, s32, s32);
extern char *func_800AE674(void *);
extern s32 func_800AF28C(void *, void *);
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
extern s32 func_80114330(void *, void *);
extern void func_801146A4(void *, void *, u16);
extern void func_80114D50(void *, s32, s32);
extern void func_8011541C(void *);
static inline s32 value(Collection *list) { return list->table_04->value_14((u8 *)list + list->table_04->delta_10); }
static inline s32 status(Actor *actor) { return actor->table_24->status_14((u8 *)actor + actor->table_24->delta_10); }
static inline Item *apply(Item *item, Actor *actor, Item *target) { return item->table_08->apply_44((u8 *)item + item->table_08->delta_40, actor, target); }
static inline Item *get(Collection *list, u32 index) { return list->table_04->get_3C((u8 *)list + list->table_04->delta_38, index); }
static inline void makeDropEvent(Event *message, Actor *source, Pos *position) {
    message->kind = 0xE;
    message->source = source;
    message->pos = *position;
}
s32 func_80114E28(Item *self, Event *event) {
    /* ODD_C: event cases are exclusive and share one original 32-byte scratch slot. */
    union { char name[0x20]; Event message; } scratch;
    void *inventory = func_8011422C(self);
    switch (event->kind) {
    case 10: {
        Item *item = event->target;
        Actor *actor = event->source;
        Collection *list;
        s32 before, free;
        if (!func_80114330(self, item)) return 0;
        list = &self->collection_0C;
        if ((actor->flags_1E >> 2) & 1) {
            if (!menu_active(D_80140160)) {
                char *name = func_800AE674(self);
                func_800498E4(0x93, name, func_800AE674(item));
                func_80049CB4(6);
                func_80049CB4(0x10D3, item);
                func_80049CB4(7);
            } else func_80049CB4(0x10D3, item);
            func_80049CB4(0x3A, actor);
            list = &self->collection_0C;
        }
        before = value(list);
        free = func_800CD278(list);
        item = apply(self, actor, item);
        if (item) func_800CD5C0(inventory, item);
        func_80114D50(self, before, free);
        if (menu_active(D_80140160)) func_80049CB4(0x138);
        return 1;
    }
    case 11: {
        s32 changed = 0;
        Actor *actor = event->source;
        Collection *from = event->target;
        s32 before, free;
        u32 index = 0;
        func_80083C90(scratch.name, func_800AE674(self));
        before = value(&self->collection_0C);
        free = func_800CD278(&self->collection_0C);
        while (index < (u32)from->table_04->count_24((u8 *)from + from->table_04->delta_20)) {
            Item *item;
            Item *result;
            s32 blocked;
            item = get(from, index);
            blocked = 0;
            if (!from->table_04->accept_64((u8 *)from + from->table_04->delta_60, item, 0) ||
                !func_800CD508(inventory, item) || item->kind == 9 || item->kind == 0x11) blocked = 1;
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
        if (self->flags_28 && (!((target->flags_1E >> 2) & 1) || !status(target))) active = 1;
        if (active) {
            Actor *source;
            makeDropEvent(&scratch.message, event->source, &target->pos);
            self->table_08->event_3C((u8 *)self + self->table_08->delta_38, &scratch.message);
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
        if (self) self->table_08->destroy_0C((u8 *)self + self->table_08->delta_08, 3);
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

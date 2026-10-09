#include "common.h"

typedef unsigned char u8;
typedef short s16;

/* Collection (item family, vtable at +4): slot +0x60/+0x64 = s32 (void *self, void *item, s32 verbose). */
typedef struct { s16 delta; s16 index; s32 (*func)(void *self, void *item, s32 verbose); } AddEntry;
typedef struct { char pad0[0x60]; AddEntry add_60; } CollectionVTable;
typedef struct { char pad0[4]; CollectionVTable *vtable; } Collection;
typedef struct { u8 kind; u8 subtype; } Item;
typedef struct { Collection *collection; Item *item; } Slot;
typedef struct { char pad0[8]; Slot slot; } Command;
typedef struct { u32 pad0 : 8; u32 flag : 1; u32 pad9 : 23; } Bits;
typedef struct { char pad0[0x20]; Bits flags_20; } Actor;
typedef struct { s32 x, y; } Pair;

extern Actor *D_801476B8;
extern s32 func_800A692C(Actor *actor, s32 kind);
extern void func_800498E4(s32 id, ...);
extern Pair *func_800E93D4(Pair *out, void *source, u8 *state);
extern s32 func_80049CB4(s32 id, ...);
extern char *func_800AE674(Item *item);
extern void *func_800D02AC(Slot *slot);
extern void func_801269C4(Item *item, void *target);
extern s32 func_800AD8AC(Item *item, Pair *position);

static inline s32 testFlag(Bits *b) { return b->flag; }

/* Command +0x14 run: take the selected item, announce it and apply it at the actor's position. */
s32 func_800DB9DC(Command *command) {
    Slot *slot = &command->slot;
    Collection *collection = slot->collection;
    AddEntry *add = &collection->vtable->add_60;
    s32 failed = add->func((char *)collection + add->delta, slot->item, 1) != 1;
    Actor *actor;
    Bits bits;
    s32 blocked;
    Item *item;
    Pair position;

    if (failed) return 0;
    actor = D_801476B8;
    bits = actor->flags_20;
    blocked = 0;
    if (!testFlag(&bits)) blocked = func_800A692C(actor, 0x12) != 0;
    if (blocked) {
        func_800498E4(0x113);
        return 0;
    }
    item = slot->item;
    func_800E93D4(&position, actor, &item->kind);
    if ((position.y | position.x) == 0) {
        func_800498E4(0x7B);
        return 1;
    }
    if (item->kind == 10) {
        func_80049CB4(0x28, actor);
    } else {
        func_80049CB4(0x27, actor);
    }
    func_800498E4(0x7C, func_800AE674(item));
    func_800D02AC(&command->slot);
    if (item->subtype == 0xE7) func_801269C4(item, (char *)actor + 8);
    func_800AD8AC(item, &position);
    return item->kind == 0x10;
}


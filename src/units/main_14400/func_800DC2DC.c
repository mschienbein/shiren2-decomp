#include "common.h"

typedef short s16;

/* Collection (item family, vtable at +4): slot +0x60/+0x64 = s32 (void *self, void *item, s32 verbose). */
typedef struct {
    s16 delta;
    s16 index;
    s32 (*func)(void *self, void *item, s32 verbose);
} AddEntry;

typedef struct {
    char pad0[0x60];
    AddEntry add_60;
} CollectionVTable;

typedef struct {
    char pad0[4];
    CollectionVTable *vtable;
} Collection;

/* Item message handler (table at +8): slot +0x38/+0x3C = s32 (void *receiver, void *event). */
typedef struct {
    s16 delta;
    s16 index;
    s32 (*func)(void *receiver, void *event);
} HandlerEntry;

typedef struct {
    char pad0[0x38];
    HandlerEntry handle_38;
} HandlerVTable;

typedef struct {
    char pad0[8];
    HandlerVTable *vtable;
} Item;

/* 0x20-byte event: id, player, target, unused tail. */
typedef struct {
    s32 id;
    void *player;
    void *target;
    s32 pad[5];
} Message;

typedef struct {
    Collection *collection;
    Item *item;
} Link;

typedef struct {
    char pad0[8];
    Link link;
} Obj;

extern void *D_801476B8;
extern void func_800DAD20(Obj *obj, Item *item);
extern void func_800DAD80(Obj *obj);

static inline void Message_setPlayer(Message *msg, void *player) {
    msg->player = player;
    msg->target = 0;
}

s32 func_800DC2DC(Obj *obj) {
    Link *link = &obj->link;
    Collection *collection = link->collection;
    AddEntry *add = &collection->vtable->add_60;
    HandlerEntry *handle;
    Item *item;
    Message msg;

    s32 ok = add->func((char *)collection + add->delta, link->item, 1) == 1;

    if (!ok) {
        return 0;
    }
    item = link->item;
    func_800DAD20(obj, item);
    msg.id = 10;
    Message_setPlayer(&msg, D_801476B8);
    handle = &item->vtable->handle_38;
    handle->func((char *)item + handle->delta, &msg);
    func_800DAD80(obj);
    return 0;
}

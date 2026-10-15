#include "common.h"
#include "collection_iterator.h"
#include "item_records.h"

typedef unsigned char u8;
typedef short s16;

typedef struct Message800FE3FC Message800FE3FC;


typedef struct {
    s16 delta;
    s16 index;
    void *(*func)(void *self);
} TargetVtableEntry;

typedef struct {
    char pad0[0x98];
    TargetVtableEntry entry98;
} TargetVtable;

typedef struct {
    char pad0[0x24];
    TargetVtable *vtable;
} Target800FE3FC;

struct Message800FE3FC {
    ItemMessage base;
    char pad4[0x18 - 0x4];
    s32 unk18;
    s32 unk1C;
};


extern s32 func_800F0EC4(void *self);
extern s32 func_800E0F40(void *self);
extern void *func_800E8A68(Target800FE3FC *target, u8 slot);
extern void func_800498E4(s32 id, ...);

void func_800FE3FC(void *self, Target800FE3FC *target) {
    Message800FE3FC msg;
    Message800FE3FC *msgp;
    CollectionIterator iter;
    Item *item;
    void *list;
    s32 found;

    if ((func_800F0EC4(self) ^ 1) == 0) {
        return;
    }
    msgp = &msg;
    msgp->base.kind = 0x19;
    msgp->unk18 = 0;
    if ((u8)func_800E0F40(self) == 1) {
        item = func_800E8A68(target, 4);
        if (item != 0 && item->field08->handle3C((char *)item + item->field08->delta38, &msgp->base) != 0) {
            func_800498E4(0x133, func_800AE674(item));
        }
        return;
    }
    list = target->vtable->entry98.func((char *)target + target->vtable->entry98.delta);
    if (list == 0) {
        return;
    }
    func_800CEB20(&iter, list);
    found = 0;
    while (func_800CEBA0(&iter) != 0) {
        Item *entry = func_800CEC68(&iter);
        s32 hit = 0;

        if ((u8)func_800E0F40(self) == 3 || (entry->flags02 & 4)) {
            if (entry->field08->handle3C((char *)entry + entry->field08->delta38, &msg.base) != 0) {
                hit = 1;
            }
        }
        if (hit) {
            found = 1;
        }
    }
    if (found) {
        func_800498E4((u8)func_800E0F40(self) < 3 ? 0x134 : 0x135);
    }
}

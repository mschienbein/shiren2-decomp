#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;

#define NULL ((void *)0)

typedef struct {
    u8 field_0;
    u8 field_1;
} Item;

typedef struct {
    u8 data[0x78];
} Slot;

typedef struct Manager {
    u8 pad0[0x100];
    Slot slots[8];
    s32 field_4C0;
    s32 field_4C4;
} Manager;

Item *func_8008CED4(Slot *);
void func_8008C9F4(Slot *);
void func_8008C75C(Item *);
s32 func_8008C89C(Item *);

void func_8008D2A8(Manager *mgr, s32 index, s32 force) {
    Slot *slot = &mgr->slots[index];
    Item *item = func_8008CED4(slot);

    func_8008C9F4(slot);
    mgr->field_4C4--;
    if (item != NULL && item->field_1 == 0) {
        if (force) {
            func_8008C75C(item);
            mgr->field_4C0--;
        } else if (func_8008C89C(item)) {
            mgr->field_4C0--;
        }
    }
}

#include "common.h"

typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;

typedef struct Obj8009A0EC Obj8009A0EC;

typedef struct {
    s16 delta;
    s16 index;
    s32 (*func)(Obj8009A0EC *self, void *arg);
} VtEntry8009A0EC;

struct Obj8009A0EC {
    u8 pad0[0x34];
    u8 unk34[0x4C - 0x34];
    VtEntry8009A0EC *vtable;
};

typedef struct {
    s32 unk0;
    void *unk4;
} Slot8009A0EC;

s32 func_8009A038(Obj8009A0EC *self);
Slot8009A0EC func_8009A054(void *obj, s32 index);
s32 func_8009A1AC(Obj8009A0EC *self, void *item);
void *func_800980F0(void *c, s32 idx);

static inline void *slotItem(Slot8009A0EC *slot) {
    return slot->unk4;
}

s32 func_8009A0EC(Obj8009A0EC *self) {
    s32 count = func_8009A038(self);
    s32 result = 0;

    if (count > 0) {
        s32 i = 0;

        for (;;) {
            Slot8009A0EC slot;

            if (!(i < count)) {
                break;
            }
            slot = func_8009A054(self, i);
            result |= func_8009A1AC(self, slotItem(&slot));
            i++;
        }
    } else {
        s32 v = self->vtable[15].func((Obj8009A0EC *)((u8 *)self + self->vtable[15].delta), self->unk34);
        result = func_8009A1AC(self, func_800980F0(self, v));
    }
    return result;
}

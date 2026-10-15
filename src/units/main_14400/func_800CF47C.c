#include "common.h"
#include "collection_iterator.h"
typedef unsigned char u8;
typedef struct { s32 x; s32 y; } Position;
/* Shared storage used by func_800CEBA0/func_800CEC68. */
typedef struct { u8 pad_0[2]; u8 flags_2; } Item;
typedef struct { u8 pad_0[0x10]; Position *pos_10; } List;
void func_800CD304(List *list, u32 index);
s32 func_80049CB4(s32 id, ...);
s32 func_800ADC90(void *object, Position *position, void *origin);

static inline void copyPosition(Position *dst, Position *src) {
    dst->x = src->x;
    dst->y = src->y;
}

/* Drop every item of the list at the list's position; true if any landed. */
s32 func_800CF47C(List *self) {
    Position pos;
    CollectionIterator it;
    s32 found = 0;
    copyPosition(&pos, self->pos_10);
    func_800CEB20(&it, self);
    while (func_800CEBA0(&it)) {
        /* The landing position doubles as the drop origin. */
        Position *p = &pos;
        Item *item;
        func_800CD304(self, (u32)it.index);
        item = func_800CEC68(&it);
        item->flags_2 |= 0x40;
        func_80049CB4(6);
        if (func_800ADC90(item, p, p)) {
            found = 1;
        }
        func_80049CB4(7);
    }
    return found;
}

#include "common.h"
typedef unsigned char u8;
typedef signed char s8;
typedef signed short s16;
typedef struct { u8 pad[0x18]; s16 delta; s16 idx; s32 (*fn)(void *, s32); } VTable;
typedef struct { u8 kind; u8 unk1; u8 flags; u8 unk3; u8 unk4; s8 owner; u8 pad6[2]; VTable *vt; } Item;
typedef struct { u8 pad[0x10]; } Iter;
s32 func_800CD4C4(void *container, void *item);
void *func_8011422C(Item *item);
Iter *func_800CEB20(Iter *it, void *list);
s32 func_800CEBA0(Iter *it);
Item *func_800CEC68(Iter *it);
static inline s32 isUnowned(Item *item) { return item->owner == -1; }
static inline s32 isReady(void *p, Item *item) { return func_800CD4C4(p, item) == 1; }
s32 func_800F6F94(u8 *self, Item *item) {
    u8 kind;
    s32 result;
    Iter it;
    if (!isReady(self + 0x8C, item)) return 0;
    kind = item->kind;
    if (kind == 9) {
        func_800CEB20(&it, func_8011422C(item));
        while (func_800CEBA0(&it)) {
            if (!isUnowned(func_800CEC68(&it))) return 0;
        }
    }
    result = 0;
    if (!item->vt->fn((u8 *)item + item->vt->delta, 0x23) && item->unk3 == 2 &&
        kind != 10 && kind != 13 && kind != 14 && kind != 15 && kind != 16 && kind != 19 &&
        !(item->flags & 0x20)) {
        result = isUnowned(item);
    }
    return result;
}
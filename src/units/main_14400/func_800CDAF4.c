#include "common.h"
typedef unsigned char u8;
/* Item vtable slot +0x18/+0x1C: s32 predicate(self, s32 kind) (e.g. func_801171DC). */
typedef struct {
    u8 pad_00[0x18];
    short adjust_18;
    unsigned short reserved_1A;
    s32 (*test_1C)(void *self, s32 kind);
} ItemVTable;
typedef struct { u8 field0, field1; u8 pad2[3]; signed char field5; u8 pad6[2]; ItemVTable *field8; } Item;
/* List vtable slots (D_80154390): +0x24 func_800CE710 s32 count(self); +0x3C func_800CE7A0
 * void *get(self, u32 index). */
typedef struct {
    u8 pad_00[0x20];
    short adjust_20;
    unsigned short reserved_22;
    s32 (*count_24)(void *self);
    u8 pad_28[0x38 - 0x28];
    short adjust_38;
    unsigned short reserved_3A;
    void *(*get_3C)(void *self, u32 index);
} ListVTable;
typedef struct { void *pool0; ListVTable *field4; } Object;
static inline s32 invalid(Item *item) { return ~item->field5; }
static inline Item *find_key(Object *obj, u8 key) {
    s32 index = obj->field4->count_24((u8 *)obj + obj->field4->adjust_20) - 1;
    for (;;) {
        Item *item;
        if (index < 0) break;
        item = obj->field4->get_3C((u8 *)obj + obj->field4->adjust_38, (u32)index--);
        if (item->field1 != key) continue;
        if (!invalid(item)) return item;
    }
    return 0;
}
Item *func_800CDAF4(Object *obj, Item *source) {
    if (!source->field8->test_1C((u8 *)source + source->field8->adjust_18, 30)) return 0;
    if (invalid(source)) return 0;
    return find_key(obj, source->field1);
}

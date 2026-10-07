#include "common.h"
typedef struct { unsigned char pad[0x18]; short field18; s32 (*field1C)(void *, s32); } ItemMethods;
typedef struct { unsigned char pad[2]; unsigned char field2; unsigned char pad3[5]; ItemMethods *field8; } Item;
typedef struct { unsigned char pad[0x60]; short field60; s32 (*field64)(void *, Item *, s32); } OwnerMethods;
typedef struct { s32 field0; OwnerMethods *field4; } Owner;
typedef struct { Owner *field0; Item *field4; } Pair;
typedef struct { unsigned char pad[8]; Pair field8; } Object;
extern void *D_801476B8;
extern void func_800AE518(void *obj, void *source, s32 arg2, s32 arg3);
static inline s32 item_flag(Item *item) { return item->field2 & 4; }
s32 func_800DB130(Object *p) {
    Pair *pair = &p->field8;
    Owner *owner = pair->field0;
    OwnerMethods *methods = owner->field4;
    Item *item;
    ItemMethods *item_methods;
    s32 result = methods->field64((unsigned char *)owner + methods->field60, pair->field4, 1) ^ 1;
    if (result) return 0;
    item = pair->field4;
    func_800AE518(item, D_801476B8, item_flag(item) == 0, 1);
    item_methods = item->field8;
    return item_methods->field1C((unsigned char *)item + item_methods->field18, 0x1E) != 0;
}

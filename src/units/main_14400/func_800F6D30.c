#include "common.h"
typedef struct { unsigned char pad_00[8]; short offset_08; short pad_0A; s32 (*method_0C)(void *); unsigned char pad_10[8]; short offset_18; short pad_1A; void (*method_1C)(void *, s32); short offset_20; short pad_22; s32 (*method_24)(void *); } Methods;
typedef struct { s32 field_00; Methods *field_04; } Collection;
typedef struct { unsigned char pad_00[8]; short offset_08; short pad_0A; void (*method_0C)(void *, s32); } ChildMethods;
typedef struct { unsigned char pad_00[8]; ChildMethods *field_08; } Child;
typedef struct { unsigned char pad_00[0x60]; short offset_60; short pad_62; void (*method_64)(void *); } ObjectMethods;
typedef struct { unsigned char pad_00[0x1C]; unsigned short field_1C; unsigned char pad_1E[6]; ObjectMethods *field_24; unsigned char pad_28[0x50]; u32 field_78; unsigned char pad_7C[0x10]; Collection field_8C; unsigned char pad_94[0x10]; s32 field_A4; volatile s32 field_A8; unsigned char field_AC; } Object;
extern unsigned char D_80156A53, D_80156A55, D_80142F1B;
extern volatile unsigned char D_80142F24;
extern unsigned char D_80147620[];
extern s32 func_800C5844(void *object, unsigned char a, unsigned char b);
extern Child *func_800AAE50(s32 value);
extern s32 func_800CD4C4(Collection *collection, Child *child);
extern s32 func_800CD538(Collection *collection, Child *child);
static inline s32 special_mode(void) { return D_80142F24 == 20; }
static inline s32 second_flag(void) { return (D_80142F1B >> 2) & 1; }
static inline Collection *reset_collection(Collection *collection) {
    s32 value = collection->field_04->method_0C((unsigned char *)collection + collection->field_04->offset_08);
    collection->field_04->method_1C((unsigned char *)collection + collection->field_04->offset_18, value);
    return collection;
}
void func_800F6D30(Object *object, unsigned char mode) {
    Collection *collection;
    s32 count;
    s32 has_items;
    object->field_1C |= 0x80;
    collection = reset_collection(&object->field_8C);
    count = (unsigned char)func_800C5844(D_80147620, D_80156A53, D_80156A55);
    for (;;) {
        Child *child;
        s32 failed;
        if (--count == -1) break;
        child = func_800AAE50(-1);
        if (!child) break;
        failed = func_800CD4C4(collection, child) != 1;
        if (!failed) {
            func_800CD538(collection, child);
        } else {
            child->field_08->method_0C((unsigned char *)child + child->field_08->offset_08, 3);
            break;
        }
    }
    object->field_A4 = 0;
    has_items = 0;
    if (mode >= 2) {
        Collection *current = &object->field_8C;
        has_items = current->field_04->method_24((unsigned char *)current + current->field_04->offset_20) != 0;
    }
    object->field_A8 = has_items;
    if (special_mode()) object->field_AC = 1;
    else if (second_flag()) object->field_AC = 2;
    else object->field_AC = 0;
    object->field_78 |= 0x04000000;
    object->field_24->method_64((unsigned char *)object + object->field_24->offset_60);
}

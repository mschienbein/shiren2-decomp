#include "common.h"
typedef struct { unsigned char pad_0[0x68]; short field_68; short field_6A; s32 (*field_6C)(void *, void *, void *, s32); } Methods;
typedef struct { unsigned char pad_0[4]; Methods *field_4; } Object;
/* Slot 0x6C targets 800CD01C and 800CFE40: collection, two item pointers, flag. */
s32 func_800CE4C4(Object *object, void *first, void *second, s32 flag) {
    return object->field_4->field_6C((unsigned char *)object + object->field_4->field_68, first, second, flag);
}

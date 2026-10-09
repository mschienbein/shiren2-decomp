#include "common.h"
typedef unsigned char u8;
typedef struct { u8 pad_00[0x90]; short adjust_90; short pad_92; s32 (*method_94)(void *, s32, s32, u8, s32); } VTable;
typedef struct { u8 pad_00[0x24]; const VTable *field_24; } Object;
/* The corresponding concrete slot is func_800F212C. */
s32 func_800E319C(Object *object) {
    return object->field_24->method_94((char *)object + object->field_24->adjust_90, 1, 1, 0, 0);
}

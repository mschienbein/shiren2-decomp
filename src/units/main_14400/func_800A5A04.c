#include "common.h"
typedef struct { s32 words[4]; } Value;
typedef struct { unsigned char pad_00[0xC]; Value field_0C; unsigned char pad_1C[2]; unsigned char field_1E; } Object;
/* Both return their rectangle by value through the hidden result pointer. */
extern Value func_800B3080(Object *object);
extern Value func_800B3024(Object *object);
static inline s32 variant(Object *object) {
    return (object->field_1E >> 1) & 1;
}
void func_800A5A04(Object *object) {
    Value value;
    if (variant(object)) value = func_800B3080(object);
    else value = func_800B3024(object);
    object->field_0C = value;
}

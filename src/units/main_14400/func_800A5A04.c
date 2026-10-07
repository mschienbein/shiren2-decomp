#include "common.h"
typedef struct { s32 words[4]; } Value;
typedef struct { unsigned char pad_00[0xC]; Value field_0C; unsigned char pad_1C[2]; unsigned char field_1E; } Object;
extern void *func_800B3080(Value *value, Object *object);
extern void *func_800B3024(Value *value, Object *object);
static inline s32 variant(Object *object) {
    return (object->field_1E >> 1) & 1;
}
void func_800A5A04(Object *object) {
    Value value;
    if (variant(object)) func_800B3080(&value, object);
    else func_800B3024(&value, object);
    object->field_0C = value;
}

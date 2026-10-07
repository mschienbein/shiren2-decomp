#include "common.h"
typedef struct { s32 field_00; s32 field_04; } Value;
typedef struct { unsigned char pad_00[0x58]; Value *field_58; } Object;
extern s32 func_800F0EC4(Object *object);
extern s32 func_800B502C(Value *value);
extern s32 func_800A650C(Object *object, Value *value);
extern s32 func_800C587C(void *object, unsigned char index);
extern unsigned char D_80147620[];
static inline void copy_value(Value *destination, Value *source) {
    destination->field_00 = source->field_00;
    destination->field_04 = source->field_04;
}
s32 func_800F0F84(Object *object, unsigned char index) {
    Value value;
    if (func_800F0EC4(object)) return 0;
    if (object->field_58) {
        s32 matches = 0;
        copy_value(&value, object->field_58);
        if (func_800B502C(&value)) matches = func_800A650C(object, &value) == 1;
        if (matches) return 0;
    }
    return func_800C587C(D_80147620, index);
}

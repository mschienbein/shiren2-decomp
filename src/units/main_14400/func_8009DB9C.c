#include "common.h"
typedef unsigned char u8;
typedef struct { s32 field_00; s32 field_04; } Value;
typedef struct { unsigned char pad_00[0x34]; Value field_34; unsigned char pad_3C[0x15]; unsigned char field_51; unsigned char field_52; unsigned char field_53; } Object;
extern u8 *func_8006A810(void *destination, s32 byte, s32 length);
extern void func_800487C4(Object *object);
extern void func_800488F0(Object *object, Value *value, s32 flags, Value *previous);
void func_8009DB9C(Object *object, Value *value) {
    if (object->field_53) {
        Value current;
        Value previous;
        Value temporary;
        s32 old;
        func_8006A810(&previous, 0, 8);
        previous.field_00 = value->field_00;
        previous.field_04 = value->field_04 % object->field_51;
        current = previous;
        func_8006A810(&temporary, 0, 8);
        temporary.field_00 = object->field_34.field_00;
        temporary.field_04 = object->field_34.field_04 % object->field_51;
        previous = temporary;
        old = object->field_34.field_04;
        object->field_34 = *value;
        if (object->field_34.field_04 != old) func_800487C4(object);
        func_800488F0(object, &current, object->field_52, &previous);
    }
}

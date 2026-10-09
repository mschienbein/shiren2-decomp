#include "common.h"
typedef struct { s32 field_0; s32 field_4; } Pair;
typedef struct { unsigned char pad_0[0x34]; Pair field_34; } Obj800487C4;
extern void func_800487C4(Obj800487C4 *);
void func_80096DD0(Obj800487C4 *object, Pair *value) {
    s32 previous = object->field_34.field_4;
    object->field_34 = *value;
    if (object->field_34.field_4 != previous) func_800487C4(object);
}

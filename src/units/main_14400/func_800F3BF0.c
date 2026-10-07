#include "common.h"

typedef short s16;
typedef struct { char pad0[0x9A]; s16 field_9A; } Obj;

void func_800F3BF0(Obj *obj, s32 value) {
    obj->field_9A = value;
}

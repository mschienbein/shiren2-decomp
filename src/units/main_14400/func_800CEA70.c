#include "common.h"

typedef struct { char pad0[0xE]; unsigned char field_E; } Obj;

void func_800CEA70(Obj *obj, s32 value) {
    obj->field_E = value;
}

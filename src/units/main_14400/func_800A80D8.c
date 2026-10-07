#include "common.h"

typedef struct {
    char pad0[0x1F];
    unsigned char field_1F;
} Obj;

void func_800A80D8(Obj *obj, s32 value) {
    obj->field_1F = value;
}

#include "common.h"

typedef struct {
    char pad0[0x3];
    unsigned char field_3;
} Obj;

void func_800AF5F0(Obj *obj, s32 value) {
    obj->field_3 = value;
}

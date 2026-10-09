#include "common.h"

typedef struct {
    unsigned char pad_00[0xD];
    unsigned char field_0D;
} Obj;

void func_80114018(Obj *obj, s32 flags) {
    obj->field_0D |= flags;
}

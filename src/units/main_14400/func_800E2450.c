#include "common.h"

typedef struct {
    unsigned char pad_00[0x54];
    unsigned char field_54;
} Obj;

void func_800E2450(Obj *obj) {
    obj->field_54 |= 8;
}

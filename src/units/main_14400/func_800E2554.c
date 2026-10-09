#include "common.h"

typedef struct { s32 x, y; } Pair;
typedef struct {
    Pair field_00;
    unsigned char pad_08[0x54];
    Pair field_5C;
} Obj;

void func_800E2554(Obj *obj) {
    obj->field_5C = obj->field_00;
}

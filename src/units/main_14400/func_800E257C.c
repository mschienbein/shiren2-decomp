#include "common.h"

typedef unsigned char u8;

typedef struct {
    u8 pad0[0x76];
    u8 field_76;
} Obj800E257C;

void func_800E257C(Obj800E257C *obj, s32 amount) {
    obj->field_76 += amount;
}

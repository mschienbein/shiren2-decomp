#include "common.h"

typedef struct { char pad0[0x96]; unsigned short field_96; } Obj;

s32 func_800F3C00(Obj *obj) {
    return obj->field_96;
}

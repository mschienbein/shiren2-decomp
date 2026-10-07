#include "common.h"

typedef struct { char pad0[0x89]; unsigned char field_89; } Obj;

s32 func_8010855C(Obj *obj) {
    return obj->field_89;
}

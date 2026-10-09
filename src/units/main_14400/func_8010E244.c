#include "common.h"

typedef struct {
    char pad0[0xC];
    s32 field_C;
} Obj;

s32 func_8010E244(Obj *obj) {
    return obj->field_C;
}

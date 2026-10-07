#include "common.h"

typedef struct {
    char pad0[0xFC];
    s32 field_FC;
} Obj;

s32 func_8009FD44(Obj *obj) {
    return obj->field_FC;
}

#include "common.h"

typedef struct {
    char pad0[0x10];
    unsigned char field_10;
} Obj_801247E4;

s32 func_801247E4(Obj_801247E4 *obj) {
    return obj->field_10 != 0;
}

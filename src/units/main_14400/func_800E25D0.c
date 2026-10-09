#include "common.h"

typedef struct {
    char pad0[0x72];
    unsigned char flags_72;
} Obj_800E25D0;

s32 func_800E25D0(Obj_800E25D0 *obj) {
    s32 bit = obj->flags_72 & 8;

    return bit != 0;
}

#include "common.h"

typedef unsigned char u8;

typedef struct {
    u8 pad0[0x94];
    u8 flags_94;
} Obj_800EE194;

s32 func_800EE194(Obj_800EE194 *obj) {
    return (obj->flags_94 >> 2) & 1;
}

#include "common.h"

typedef unsigned char u8;

typedef struct {
    u8 pad0[0xD];
    u8 field_D;
} Obj800CEA78;

s32 func_800CEA78(Obj800CEA78 *obj) {
    return obj->field_D;
}

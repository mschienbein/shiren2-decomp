#include "common.h"
typedef unsigned char u8;
typedef struct { s32 x; s32 y; } Pair800A3834;
typedef struct { u8 pad0[0x8]; s32 field_8; s32 field_C; } Obj800A3834;
Pair800A3834 *func_800A3834(Pair800A3834 *out, Obj800A3834 *obj) {
    out->x = obj->field_8;
    out->y = obj->field_C;
    return out;
}

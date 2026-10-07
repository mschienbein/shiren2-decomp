#include "common.h"

typedef unsigned char u8;

/* Two parallel 4-column byte tables: base at +0x00, override at +0x43. */
typedef struct {
    u8 base[0x43];
    u8 override[0x14];
} Obj;

s32 func_800D1A00(Obj *obj, s32 row, s32 col) {
    s32 i = col + row * 4;
    u8 value = obj->override[i];
    if (value == 0) {
        value = obj->base[i];
    }
    return value;
}

#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

typedef struct {
    u8 pad0[0x32];
    u8 field_32;
    u8 pad33[0xA];
    u8 field_3D;
} Obj800E0F40;

s32 func_800E0F40(Obj800E0F40 *obj) {
    if (obj->field_3D != 0) {
        return (obj->field_32 + 1) >> 1;
    }
    return obj->field_32;
}

#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

typedef struct {
    u8 pad0[0x1C];
    u16 flags_1C;
} Obj_800A8224;

void func_800A8224(Obj_800A8224 *obj) {
    obj->flags_1C |= 0x20;
}

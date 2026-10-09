#include "common.h"

typedef unsigned char u8;

typedef struct {
    u8 pad0[0x2C];
    u8 field_2C;
} Obj80122434;

void func_80122434(Obj80122434 *obj) {
    obj->field_2C = 0xFF;
}

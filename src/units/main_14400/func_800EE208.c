#include "common.h"

typedef unsigned char u8;

typedef struct {
    u8 pad0[0x94];
    u8 unk94;
} Obj800EE208;

void func_800EE208(Obj800EE208 *obj) {
    obj->unk94 &= ~8;
}

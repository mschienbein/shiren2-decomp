#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

typedef struct { u8 pad0[0x1C]; u16 flags_1C; } Obj800A815C;

void func_800A815C(Obj800A815C *obj) {
    obj->flags_1C &= ~0x200;
}

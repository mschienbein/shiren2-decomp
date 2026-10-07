#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

typedef struct { u8 pad0[0xE4]; u16 flags; } Obj800EE0D4;

void func_800EE0D4(Obj800EE0D4 *obj) {
    obj->flags |= 0x80;
}

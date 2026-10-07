#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef struct { u8 pad0[0xE4]; u16 flags_E4; } Obj800EE114;

void func_800EE114(Obj800EE114 *obj) {
    obj->flags_E4 |= 0x80;
}

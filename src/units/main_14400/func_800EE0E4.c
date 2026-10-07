#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

typedef struct { u8 pad0[0xE4]; u16 flags_E4; } Obj800EE0E4;

s32 func_800EE0E4(Obj800EE0E4 *obj) {
    return (obj->flags_E4 >> 6) & 1;
}

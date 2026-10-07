#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef signed char s8;
typedef short s16;

typedef struct { u8 pad0[0xE4]; u16 flags_E4; } Obj800EE104;

s32 func_800EE104(Obj800EE104 *obj) {
    return (obj->flags_E4 >> 7) & 1;
}

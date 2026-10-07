#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;

typedef struct { u8 pad0[0xE4]; u16 flags; } Obj800EE164;
s32 func_800EE164(Obj800EE164 *obj) {
    return (obj->flags >> 5) & 1;
}

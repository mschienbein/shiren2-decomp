#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;
typedef float f32;

typedef struct { u8 pad0[0xE4]; u16 field_E4; } Obj800EE0C4;
s32 func_800EE0C4(Obj800EE0C4 *obj) {
    return (obj->field_E4 >> 7) & 1;
}

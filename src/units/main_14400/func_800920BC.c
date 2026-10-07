#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;

typedef struct { u8 pad0[0x14]; s32 field_14; } Obj;
s32 func_800920BC(Obj *obj) {
    return obj->field_14 != 0;
}

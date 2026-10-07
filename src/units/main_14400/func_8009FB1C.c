#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;

typedef struct { u8 pad0[0x128]; s32 field_128; } Obj8009FB1C;
s32 func_8009FB1C(Obj8009FB1C *obj) {
    return obj->field_128 > 0;
}

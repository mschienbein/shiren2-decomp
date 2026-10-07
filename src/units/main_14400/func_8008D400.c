#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;
typedef float f32;

typedef struct { s32 field_0; u8 pad4[0x8]; s32 field_C; } Obj8008D400;
void func_8008D400(Obj8008D400 *obj) {
    obj->field_0 = 0;
    obj->field_C = 0;
}

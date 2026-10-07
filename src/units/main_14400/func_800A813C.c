#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;
typedef float f32;

typedef struct { u8 pad0[0x1C]; u16 field_1C; } Obj800A813C;
void func_800A813C(Obj800A813C *obj) {
    obj->field_1C |= 0x8000;
}

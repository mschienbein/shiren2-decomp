#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;
typedef float f32;

typedef struct { u8 pad0[0xC]; s32 field_C; } Obj8010E24C;
void func_8010E24C(Obj8010E24C *obj, s32 value) {
    obj->field_C = value;
}

#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;
typedef float f32;

typedef struct { u8 pad0[0xC]; u8 field_C; } Obj80128B44;
void func_80128B44(Obj80128B44 *obj) {
    obj->field_C |= 4;
}

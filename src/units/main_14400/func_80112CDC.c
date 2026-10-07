#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;

typedef struct { u8 pad0[0xC]; s32 field_C; } Obj80112CDC;
void func_80112CDC(Obj80112CDC *obj, s32 value) {
    obj->field_C = value;
}

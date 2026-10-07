#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;
typedef float f32;

typedef struct { u8 pad0[0x8C]; s32 field_8C; } Obj800EB35C;
void func_800EB35C(Obj800EB35C *obj, u8 seconds) {
    obj->field_8C = seconds * 1000;
}

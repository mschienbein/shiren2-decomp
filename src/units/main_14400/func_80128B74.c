#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;

typedef struct { u8 pad0[0xC]; u8 field_C; } Obj;
void func_80128B74(Obj *obj) {
    obj->field_C |= 2;
}

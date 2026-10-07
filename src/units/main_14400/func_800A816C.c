#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;

typedef struct { u8 pad0[0x1C]; u16 field_1C; } Obj;
void func_800A816C(Obj *obj) {
    obj->field_1C |= 0x200;
}

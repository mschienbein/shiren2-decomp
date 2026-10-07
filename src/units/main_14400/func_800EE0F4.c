#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;

typedef struct { u8 pad0[0xE4]; u16 field_E4; } Obj;
void func_800EE0F4(Obj *obj) {
    obj->field_E4 &= ~0x80;
}

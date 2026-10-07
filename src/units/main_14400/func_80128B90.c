#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef signed char s8;
typedef short s16;

typedef struct { u8 pad0[0xC]; u8 flags_C; } Obj80128B90;

void func_80128B90(Obj80128B90 *obj) {
    obj->flags_C &= ~1;
}

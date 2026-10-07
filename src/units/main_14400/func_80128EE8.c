#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;

typedef struct { u8 pad0[0xD]; u8 flags; } Obj80128EE8;
void func_80128EE8(Obj80128EE8 *obj) {
    obj->flags |= 0x80;
}

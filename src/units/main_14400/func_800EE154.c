#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;

typedef struct { u8 pad0[0xE4]; u16 flags; } Obj800EE154;
void func_800EE154(Obj800EE154 *obj) {
    obj->flags &= ~0x20;
}

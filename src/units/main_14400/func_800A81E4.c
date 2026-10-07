#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;

typedef struct { u8 pad0[0x1C]; u16 flags; } Obj800A81E4;
void func_800A81E4(Obj800A81E4 *obj) {
    obj->flags &= ~0x40;
}

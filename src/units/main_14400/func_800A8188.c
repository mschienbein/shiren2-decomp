#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef signed char s8;
typedef short s16;

typedef struct { u8 pad0[0x1C]; u16 flags_1C; } Obj800A8188;

void func_800A8188(Obj800A8188 *obj) {
    obj->flags_1C &= ~0x100;
}

#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;

typedef struct { u8 pad0[0x8]; s32 unk8; } Obj800A1DA0;
Obj800A1DA0 *func_800A1DA0(Obj800A1DA0 *obj) {
    obj->unk8 = 0;
    return obj;
}

#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef struct { u8 pad0[0xC]; u8 flags_C; } Obj80128BA0;

void func_80128BA0(Obj80128BA0 *obj) {
    obj->flags_C |= 1;
}

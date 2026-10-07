#include "common.h"

typedef unsigned char u8;

typedef struct { u8 pad0[0xC]; u8 flags_C; } Obj80128B64;

void func_80128B64(Obj80128B64 *obj) {
    obj->flags_C &= ~2;
}

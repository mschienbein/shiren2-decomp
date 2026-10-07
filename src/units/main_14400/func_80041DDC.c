#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef struct { u8 pad0[0xC]; u8 field_C; } Obj80041DDC;
typedef struct { s32 b; s32 a; } Key80041DDC;
Obj80041DDC *func_800B4D80(Key80041DDC *key);

u8 func_80041DDC(s32 a, s32 b) {
    Key80041DDC key;

    key.a = a;
    key.b = b;
    return func_800B4D80(&key)->field_C;
}

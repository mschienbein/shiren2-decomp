#include "common.h"
typedef unsigned char u8;
typedef struct { u8 pad0[0x54]; u8 flags54; } Object;
void func_800E33A4(Object *object, s32 mask) { object->flags54 &= ~mask; }

#include "common.h"
typedef unsigned char u8;
typedef struct { u8 pad0[0xC]; u8 flags0C; } Object;
void func_80116AF4(Object *object) { object->flags0C |= 0x20; }

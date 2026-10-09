#include "common.h"
typedef unsigned char u8;
typedef struct { u8 pad0[0xC]; u8 fieldC; } Object;
s32 func_80116BC0(Object *object) { return (object->fieldC >> 4) & 1; }

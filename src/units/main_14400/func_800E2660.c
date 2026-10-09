#include "common.h"
typedef unsigned char u8;
typedef struct { u8 pad0[0x72]; u8 field72; } Object;
s32 func_800E2660(Object *object, s32 mask) { return (object->field72 & mask) != 0; }

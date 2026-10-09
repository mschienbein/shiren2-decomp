#include "common.h"
typedef unsigned char u8;
typedef struct { u8 pad0[0x72]; u8 flags72; } Object;
s32 func_800E25A0(Object *object) {
    if (object->flags72 & 0x10) return 1;
    return 0;
}

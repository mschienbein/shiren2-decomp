#include "common.h"
typedef struct { unsigned char pad0[0x72]; unsigned char flags72; } Object;
s32 func_800E2630(Object *object) {
    s32 result = 0;
    if (object->flags72 & 2) result = 1;
    return result;
}

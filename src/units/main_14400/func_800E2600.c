#include "common.h"

typedef unsigned char u8;
typedef struct { u8 pad00[0x72]; u8 flags72; } Object;

s32 func_800E2600(Object *self) {
    s32 result = 0;
    if (self->flags72 & 4) result = 1;
    return result;
}

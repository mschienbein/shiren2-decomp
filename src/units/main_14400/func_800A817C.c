#include "common.h"

typedef unsigned char u8;

typedef struct {
    u8 pad0[0x1C];
    u8 unk1C;
} Obj800A817C;

s32 func_800A817C(Obj800A817C *obj) {
    return obj->unk1C & 1;
}

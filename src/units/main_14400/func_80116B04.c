#include "common.h"

typedef unsigned char u8;

typedef struct {
    u8 pad0[0xC];
    u8 flags;
} Obj80116B04;

s32 func_80116B04(Obj80116B04 *obj) {
    return (obj->flags >> 5) & 1;
}

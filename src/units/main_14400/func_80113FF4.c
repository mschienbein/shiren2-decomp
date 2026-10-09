#include "common.h"

typedef unsigned char u8;

typedef struct {
    u8 pad00[0xD];
    u8 flags0D;
} Obj80113FF4;

s32 func_80113FF4(Obj80113FF4 *obj, s32 mask) {
    return (obj->flags0D & mask) != 0;
}

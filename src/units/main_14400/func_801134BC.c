#include "common.h"

typedef unsigned char u8;

typedef struct {
    u8 pad0[0xC];
    u8 flagsC;
} Obj_801134BC;

s32 func_801134BC(Obj_801134BC *obj) {
    return obj->flagsC & 1;
}

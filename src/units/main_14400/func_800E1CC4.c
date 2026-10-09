#include "common.h"

typedef unsigned char u8;

/* Ten per-kind flags; func_800E1C58 dispatches kinds 0..9 here. */
typedef struct {
    u8 pad0[0x35];
    u8 flags35[10];
} Obj_800E1CC4;

s32 func_800E1CC4(Obj_800E1CC4 *obj, s32 kind) {
    return obj->flags35[kind] != 0;
}

#include "common.h"

typedef struct Obj800E1CC4 Obj800E1CC4;

extern s32 func_800E1CC4(Obj800E1CC4 *obj, s32 kind);

/* Flag query for kind 5 (byte 0x35 + kind is nonzero). */
s32 func_800E2FB0(Obj800E1CC4 *obj) {
    return func_800E1CC4(obj, 5);
}

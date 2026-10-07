#include "common.h"

typedef struct {
    s32 unk0;
    s32 unk4;
} Pair80102E80;

typedef struct {
    char pad0[0xA0];
    s32 unkA0;
    s32 unkA4;
} Obj80102E80;

Pair80102E80 *func_80102E80(Pair80102E80 *out, Obj80102E80 *obj) {
    out->unk0 = obj->unkA0;
    out->unk4 = obj->unkA4;
    return out;
}

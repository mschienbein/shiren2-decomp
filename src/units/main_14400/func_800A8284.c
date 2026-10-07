#include "common.h"

typedef unsigned short u16;

typedef struct {
    char pad0[0x1C];
    u16 unk1C;
} Obj800A8284;

void func_800A8284(Obj800A8284 *obj) {
    obj->unk1C |= 8;
}

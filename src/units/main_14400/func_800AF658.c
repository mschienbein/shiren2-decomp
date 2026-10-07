#include "common.h"

typedef struct { unsigned char pad0[5]; signed char unk5; } Obj;

void func_800AF658(Obj *obj) {
    obj->unk5 = -1;
}

#include "common.h"

typedef struct { unsigned char pad0[0xE4]; unsigned short unkE4; } Obj;

void func_800EE2B4(Obj *obj) {
    obj->unkE4 &= 0xFFF0;
}

#include "common.h"

typedef struct { unsigned char unknown00[2]; unsigned char flags02; } Object;
void func_800AF77C(Object *object, u32 mask) { object->flags02 &= ~mask; }

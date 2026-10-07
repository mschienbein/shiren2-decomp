#include "common.h"

typedef struct { char pad[2]; unsigned char flags; } Object;
void func_800AF74C(Object *obj) { obj->flags &= ~4; }

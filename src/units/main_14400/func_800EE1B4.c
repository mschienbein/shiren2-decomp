#include "common.h"
typedef struct { unsigned char pad0[0x94]; unsigned char flags; } Obj800EE1B4;
void func_800EE1B4(Obj800EE1B4 *obj) { obj->flags &= ~3; }

#include "common.h"
typedef struct { unsigned char pad0[0x1C]; unsigned short flags; } Obj800A8244;
void func_800A8244(Obj800A8244 *obj) { obj->flags &= ~0x10; }

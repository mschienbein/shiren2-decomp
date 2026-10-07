#include "common.h"
typedef struct { unsigned char pad[0x20]; s32 field20; } Object;
void func_800A808C(Object *p, s32 mask) { p->field20 &= ~mask; }

#include "common.h"
typedef struct { unsigned char pad[0x9A]; unsigned short field9A; } Object;
s32 func_800F3BBC(Object *p, s32 mask) { return (p->field9A & mask) != 0; }

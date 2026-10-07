#include "common.h"
typedef struct { char pad[0x9A]; unsigned short field_9A; } Obj;
void func_800F3A38(Obj *a) { a->field_9A |= 0x200; }

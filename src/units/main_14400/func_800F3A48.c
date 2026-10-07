#include "common.h"
typedef struct { unsigned char pad[0x9A]; unsigned char field9A; } Object;
s32 func_800F3A48(Object *p) { return p->field9A & 1; }

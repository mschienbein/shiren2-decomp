#include "common.h"
typedef struct { unsigned char pad[0x124]; s32 field124; s32 field128; } Object;
s32 func_8009FB00(Object *p) { s32 count = p->field128; if (count <= 0) count = p->field124 != 0; return count; }

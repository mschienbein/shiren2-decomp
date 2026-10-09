#include "common.h"
typedef struct { unsigned char pad0[0x1E]; unsigned char flags1E; } Object;
s32 func_800A7D20(Object *object) { return (object->flags1E >> 1) & 1; }

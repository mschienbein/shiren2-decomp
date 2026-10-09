#include "common.h"
typedef struct { unsigned char pad00[0x54]; unsigned char flags54; } Object;
s32 func_800E3394(Object *object, s32 mask) { return (object->flags54 & mask) != 0; }

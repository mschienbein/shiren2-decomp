#include "common.h"
typedef unsigned char u8;
typedef struct { u8 field_00[0x1E]; u8 field_1E; } Object;
s32 func_800A7D04(Object *object, s32 mask) { return (object->field_1E & mask) != 0; }

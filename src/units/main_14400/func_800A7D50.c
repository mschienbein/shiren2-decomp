#include "common.h"
typedef unsigned char u8;
typedef struct { u8 pad0[0x1E]; u8 field1E; } Object;
s32 func_800A7D50(Object *object) { return (object->field1E >> 6) & 1; }

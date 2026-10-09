#include "common.h"
typedef struct { unsigned char pad_00[0x1E]; unsigned char field_1E; } Object;
s32 func_800A7DBC(Object *object) { return (object->field_1E & 3) != 0; }

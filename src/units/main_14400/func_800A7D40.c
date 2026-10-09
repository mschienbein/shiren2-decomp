#include "common.h"
typedef struct { unsigned char pad_00[0x1E]; unsigned char field_1E; } Object;
s32 func_800A7D40(Object *self) { return (self->field_1E >> 4) & 1; }

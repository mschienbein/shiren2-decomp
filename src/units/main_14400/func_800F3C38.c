#include "common.h"
typedef struct { unsigned char field_00[0x7c]; unsigned short field_7c; } Object;
s32 func_800F3C38(Object *self) { return (self->field_7c >> 8) & 1; }

#include "common.h"

typedef struct { unsigned char reserved_00[0x9C]; unsigned char field_9C; } Object;
s32 func_800F38C4(Object *self) { return self->field_9C; }

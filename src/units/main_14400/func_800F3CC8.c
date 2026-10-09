#include "common.h"

typedef struct { unsigned char reserved_00[0x84]; unsigned char field_84; } Object;
s32 func_800F3CC8(Object *self) { return self->field_84; }

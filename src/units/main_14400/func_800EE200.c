#include "common.h"

typedef struct { char reserved_00[0x90]; s32 field_90; } Object;
s32 func_800EE200(Object *self) { return self->field_90; }

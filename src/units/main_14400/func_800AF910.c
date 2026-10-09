#include "common.h"
/* Pool header (func_800AF890): record storage, occupancy bits, capacity at +8, free count at +0xC. */
typedef struct { void *field_0; unsigned char *field_4; s32 field_8, field_C; } Object;
void func_800AF910(Object *self, s32 value) { self->field_C = self->field_8 - value; }

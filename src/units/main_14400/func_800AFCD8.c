#include "common.h"
extern const unsigned char D_8015488C[8];
/* Pool header view: record storage pointer at +0, occupancy bitset pointer at +4. */
typedef struct { void *field_0; unsigned char *field_4; } Object;
void func_800AFCD8(Object *self, unsigned char index) { s32 bit = index; self->field_4[bit >> 3] |= D_8015488C[bit & 7]; }

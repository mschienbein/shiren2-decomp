#include "common.h"

/* Saved +0x8C selection tokens, including actual item pointers. */
typedef struct { void *entries[8]; s32 count; } Selection;
extern void *D_80140210[8];

void func_80095CC0(Selection *self, s32 count)
{
    self->count = count;
    while (--count != -1) {
        self->entries[count] = D_80140210[count];
    }
}

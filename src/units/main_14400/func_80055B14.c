#include "common.h"

/* Partial views of the two observed 32-bit storage locations. */
extern u32 D_801399B0;
extern u32 D_801399B4;

void func_80055B14(u32 word_00, u32 word_04)
{
    D_801399B0 = word_00;
    D_801399B4 = word_04;
}

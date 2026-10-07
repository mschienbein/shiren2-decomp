#include "common.h"

/* Independently reconstructed word-status wrapper; original names preserved. */

typedef char word_width[(sizeof(u32) == 4 && sizeof(s32) == 4) ? 1 : -1];
typedef char abi_width[(sizeof(int) == 4 && sizeof(void *) == 4) ? 1 : -1];

extern s32 func_80032850(u32 pc);

s32 func_80025E80(void)
{
    return func_80032850(0);
}

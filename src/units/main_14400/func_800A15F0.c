#include "common.h"

typedef unsigned char u8;
typedef struct { u8 first[5]; u8 second[4]; } Flags;

void func_800A15F0(Flags *flags)
{
    u8 *p = flags->first;
    u8 *q;
    s32 n = 4;
    do {
        *p++ = 0;
    } while (n-- > 0);
    q = flags->second;
    n = 3;
    do {
        *q++ = 0;
    } while (n-- > 0);
}

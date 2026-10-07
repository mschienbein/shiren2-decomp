#include "common.h"

typedef unsigned char u8;

/* Bump-allocator enable flag, end and cursor (pointer globals shared with func_8006A8D8). */
extern s32 D_8013CA20;
extern u8 *D_8016FD68;
extern u8 *D_8016FD64;

/* Bytes still available between the allocation cursor and the arena end. */
s32 func_8006AA08(void)
{
    if (D_8013CA20 == 0) {
        return 0;
    }
    return D_8016FD68 - D_8016FD64;
}

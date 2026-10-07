#include "common.h"

typedef unsigned char u8;

extern s32 D_8013CA20;
/* Bump-allocator cursor and saved mark (shared with func_8006A8D8/func_8006A9AC/func_8006A9D4). */
extern u8 *D_8016FD64;
extern u8 *D_8016FD6C;

void func_8006A8B0(void) {
    if (D_8013CA20 != 0) {
        D_8016FD6C = D_8016FD64;
    }
}

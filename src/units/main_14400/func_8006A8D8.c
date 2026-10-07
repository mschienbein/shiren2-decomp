#include "common.h"
typedef unsigned char u8;
extern s32 D_8013CA20;
extern u8 *D_8016FD64;
extern u8 *D_8016FD68;
/* name: caller's debug tag string; never read by this allocator. */
void *func_8006A8D8(char *name, u32 size) {
    u8 *ptr;
    size = (size + 7) & ~7;
    if (D_8013CA20 == 0) {
        return 0;
    }
    if (size < (u32)(D_8016FD68 - D_8016FD64)) {
        ptr = D_8016FD64;
        D_8016FD64 = ptr + size;
    } else {
        ptr = 0;
    }
    return ptr;
}

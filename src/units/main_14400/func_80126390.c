#include "common.h"

typedef unsigned char u8;

extern u8 D_801CA650[];
void func_80126390(void *owner) {
    s32 i;
    (void)owner; /* unused receiver kept in slot a0 */
    for (i = 25; i != -1; i--) {
        D_801CA650[i] = 0;
    }
}

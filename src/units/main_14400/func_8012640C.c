#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;

extern u8 D_801CA66A[];
u8 func_800A8C00(void *actor);
s32 func_8012640C(void *owner, void *unit) {
    u32 id = func_800A8C00(unit);
    (void)owner; /* unused receiver kept in slot a0 */
    if (id != 0xFF) {
        return (D_801CA66A[id >> 3] >> (id & 7)) & 1;
    }
    return 1;
}

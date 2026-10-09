#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;

/* Signed view of the menu-system flags byte at +5. */
extern s8 D_80140160[];
s32 func_80041E6C(void) {
    return D_80140160[5];
}

#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;

/* Actor slot +0xE4 supplies receiver, source and item pointers. This default
 * does not inspect them, unlike overrides func_8010A5B0 and func_8010AEB8. */
s32 func_800EF68C(void *self, void *source, void *item) {
    return 0;
}

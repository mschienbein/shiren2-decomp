#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;

/* Leading word of the D_80161654 state record: the scalar generated ID. */
extern s32 D_80161654;
s32 func_8012A4E4(s32 id);
s32 func_800524F4(void) {
    return func_8012A4E4(D_80161654);
}

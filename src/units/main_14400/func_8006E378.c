#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

typedef struct { u16 x0; u16 x2; u8 pad4[0x14]; u8 x18; } Pad;
extern u16 D_801A70FA;
/* D_8013D3F0 handler: func_8006D550 passes the current input record (a0) and its saved
 * input record (a1, 0x8006D848); this handler leaves `saved` unused. */
s32 func_8006E378(Pad *pad, void *saved) {
    s32 result = -1;
    if ((pad->x2 & 0xCF0F) || (pad->x18 && D_801A70FA)) {
        result = 1;
    } else if ((pad->x0 & 0xCF0F) || pad->x18) {
        result = 5;
    }
    return result;
}

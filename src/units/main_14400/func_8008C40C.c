#include "common.h"

typedef unsigned char u8;
typedef struct { u8 data[0x78]; } Entry;
extern s32 D_8013FEE0;
extern u8 *D_8013FEE4;
u8 func_8008C40C(s32 i) {
    if (D_8013FEE0 == 0) {
        return 0;
    }
    /* The record array follows the allocation's 0x100-byte header. */
    return ((Entry *)(D_8013FEE4 + 0x100))[i].data[1];
}

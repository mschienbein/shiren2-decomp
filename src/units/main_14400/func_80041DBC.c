#include "common.h"

typedef unsigned char u8;
u8 func_800AC1AC(u8);
/* func_80078908 passes a full-width s32 (daddu a0,s1 at 0x80078A6C, no andi);
 * the narrowing happens here (andi before func_800AC1AC). */
s32 func_80041DBC(s32 arg0) {
    return (u8)func_800AC1AC((u8)arg0);
}

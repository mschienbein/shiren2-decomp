#include "common.h"

extern unsigned char D_8014B890;
extern unsigned short D_8014B874[];
s32 func_80052D08(short id) {
    s32 found = 0;
    s32 i = D_8014B890;
    while (--i != -1) {
        if (id == D_8014B874[i]) { found = 1; break; }
    }
    return found;
}

#include "common.h"

typedef unsigned char u8;
typedef struct { u8 pad[0x28]; u8 f28; } S;
s32 func_8012297C(S *s, s32 kind) {
    s32 result;
    if (kind == 0x1D) {
        return s->f28 != 0;
    }
    result = 0;
    if (kind == 0xB || kind == 0x16) {
        result = 1;
    }
    return result;
}

#include "common.h"

typedef struct { s32 f0; s32 f4; } S;
s32 func_800A24A8(S *s) {
    s32 result = 0;
    if (s->f4 == 0 || s->f0 == 0 || s->f4 == s->f0 || s->f4 == -s->f0) {
        result = 1;
    }
    return result;
}

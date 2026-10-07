#include "common.h"

typedef struct {
    s32 a;
    s32 b;
} Pair;

s32 func_80041874(s32 arg0, s32 arg1) {
    Pair dst;
    Pair src;

    src.a = arg1;
    src.b = arg0;
    dst = src;
    return 0;
}

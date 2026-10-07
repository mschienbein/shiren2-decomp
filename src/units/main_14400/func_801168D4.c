#include "common.h"

extern volatile s32 D_801486C0;
extern volatile s32 D_801486C4;
extern volatile s32 D_801486C8;

s32 func_801168D4(s32 *out) {
    s32 first;
    s32 second;
    s32 third;

    first = D_801486C0;
    second = D_801486C4;
    third = D_801486C8;
    out[0] = second;
    out[1] = third;
    return first;
}

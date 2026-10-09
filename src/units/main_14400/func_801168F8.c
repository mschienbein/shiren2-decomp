#include "common.h"

typedef union {
    s32 words[2];
    struct {
        unsigned char pad0[3];
        unsigned char x;
        unsigned char pad4[3];
        unsigned char y;
    } bytes;
} Pair_801168F8;

extern s32 D_801486C0;
extern Pair_801168F8 D_801486C4;

Pair_801168F8 *func_801168F8(Pair_801168F8 *out, Pair_801168F8 *src) {
    if (D_801486C0 != 0) {
        src = &D_801486C4;
    }
    out->words[0] = src->words[0];
    out->words[1] = src->words[1];
    return out;
}

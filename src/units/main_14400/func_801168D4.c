#include "common.h"

typedef union {
    s32 words[2];
    struct {
        unsigned char pad0[3];
        unsigned char x;
        unsigned char pad4[3];
        unsigned char y;
    } bytes;
} Pos;
extern s32 D_801486C0;
extern Pos D_801486C4;

s32 func_801168D4(Pos *out) {
    s32 status = D_801486C0;
    *out = D_801486C4;
    return status;
}

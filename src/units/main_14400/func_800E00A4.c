#include "common.h"

typedef struct { unsigned char unk0; unsigned char unk1; unsigned char pad2[6]; unsigned char unk8; } Src;

s32 func_800E00A4(Src *src, unsigned char *dst) {
    dst[0] = src->unk1;
    dst[1] = src->unk8;
    return 2;
}

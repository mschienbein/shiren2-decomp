#include "common.h"

typedef unsigned char u8;

typedef struct {
    u8 unk0;
    u8 unk1;
    char pad2[6];
    char unk8[8];
    char unk10[8];
} Src800DC174;

void func_800DA9DC(u8 *dst, char *src);

s32 func_800DC174(Src800DC174 *src, u8 *dst) {
    *dst++ = src->unk1;
    func_800DA9DC(dst++, src->unk8);
    func_800DA9DC(dst, src->unk10);
    return 3;
}

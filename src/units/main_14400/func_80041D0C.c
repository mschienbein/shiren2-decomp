#include "common.h"

typedef struct { char pad[0x1E]; unsigned char unk1E; } S;
extern void *func_800A8CB0(s32 cell);
extern s32 func_800E1CD4(S *, s32);
s32 func_80041D0C(unsigned char id) {
    S *s = func_800A8CB0(id);
    if (s == 0) return 0;
    if ((s->unk1E & 0x7C) == 0) return 0;
    return func_800E1CD4(s, 10);
}

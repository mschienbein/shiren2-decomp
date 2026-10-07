#include "common.h"

typedef struct { s32 unk0; void *unk4; char pad8[0xC0]; s32 unkC8; } S;
extern char D_801589B8[];
extern void func_800DDAD0(S *, s32);
extern void func_800DDC0C(S *, unsigned char *, s32);
extern void func_800DEA54(S *);

S *func_800DE7EC(S *s, unsigned char *str) {
    s32 len;
    func_800DDAD0(s, 0x2B);
    s->unk4 = D_801589B8;
    s->unkC8 = 0;
    len = *str++;
    func_800DDC0C(s, str, len - 1);
    func_800DEA54(s);
    return s;
}

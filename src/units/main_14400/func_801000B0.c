#include "common.h"
typedef unsigned char u8;

typedef struct { char pad[0x58]; void *x58; char pad2[0x3E]; unsigned short x9A; } S;
u8 func_800A6420(void *, void *);
s32 func_800A67DC(void *, void *, s32, s32);
s32 func_800F1024(S *);
void func_800F06E4(S *);
s32 func_800E1CD4(S *, s32);
s32 func_800E7104(S *);
s32 func_800E8350(S *);
s32 func_801000B0(S *s) {
    void *t = s->x58;
    s32 r = func_800A6420(s, t);
    s32 f;
    if (r < 3 && r != 0 && (f = s->x9A & 0x40, func_800A67DC(s, t, 0, f != 0)) && func_800F1024(s)) {
        func_800F06E4(s);
        return 0;
    }
    if (func_800E1CD4(s, 0x10) != 0) return func_800E8350(s);
    return func_800E7104(s);
}

#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;

typedef struct { u8 pad[0x24]; void *vt; u8 pad2[0x78 - 0x28]; u32 f_78; } S;
extern u8 D_80159CB0[];
void *func_800F3CF0(S *s, s32 kind, u8 b);
s32 func_800A3934(S *s);
void func_800E4D88(S *s, s32 a);
void func_800F4688(S *s);
S *func_800F86B0(S *s, u8 b) {
    func_800F3CF0(s, 0x5C, b);
    s->vt = D_80159CB0;
    if (func_800A3934(s) == 0) {
        func_800E4D88(s, 0);
        s->f_78 |= 0x4000000;
        func_800F4688(s);
    }
    return s;
}

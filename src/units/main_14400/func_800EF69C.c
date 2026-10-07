#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;

typedef struct { u8 pad[0x24]; void *vt; u8 pad2[0xB4 - 0x28]; void *vt2; } S;
extern u8 D_80159130[];
extern u8 D_80159150[];
void func_800E016C(S *s, s32 flags);
void func_800A3918(S *s);
void func_800EF69C(S *s, s32 flags) {
    s->vt2 = D_80159130;
    s->vt = D_80159150;
    func_800E016C(s, 0);
    if (flags & 1) func_800A3918(s);
}

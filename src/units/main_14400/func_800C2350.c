#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef struct {
    u8 kind;
    char pad1[3];
    s32 field_4;
    void *vtable;
    char pad0C[5];
    u8 field_11;
} S;
extern char D_80153B40[];
extern char D_80153ED8[];
void func_80042EC0(S *s, s8 a, s8 b, s8 c, s8 d);
S *func_800C2350(S *s, u8 kind, s8 a, s8 b, s8 c, s8 d, s32 e) {
    s->vtable = D_80153B40;
    s->kind = kind;
    s->vtable = D_80153ED8;
    s->field_4 = e;
    func_80042EC0(s, a, b, c, d);
    s->field_11 = kind == 12 && a == 7;
    return s;
}

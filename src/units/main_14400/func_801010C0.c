#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;

typedef struct { u8 pad[0x24]; void *x24; } S;
extern u8 D_8015B440[];
void func_800EFD28(S *s, s32 flags);
void func_800A3918(S *s);
void func_801010C0(S *s, s32 flags) {
    s->x24 = D_8015B440;
    func_800EFD28(s, 0);
    if (flags & 1) {
        func_800A3918(s);
    }
}

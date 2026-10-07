#include "common.h"
typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef signed short s16;
typedef struct { u8 pad[0x58]; void *x58; } S;
s32 func_800E1CC4(S *, s32);
s32 func_800E1CD4(S *, s32);
u8 func_800A6420(void *, void *);
s32 func_800A455C(void *, void *, s32);
void func_800F06E4(S *);
s32 func_800E7104(S *);
s32 func_800E8350(S *);
s32 func_80100E28(S *s) {
    s32 mode;
    void *id = s->x58;
    s32 ok;
    mode = func_800E1CC4(s, 2) ? 1 : 3;
    ok = 0;
    if (func_800A6420(s, id) != 3) {
        ok = func_800A455C(s, id, mode) != 0;
    }
    if (ok) {
        func_800F06E4(s);
        return 0;
    }
    if (func_800E1CD4(s, 0x10)) {
        return func_800E8350(s);
    }
    return func_800E7104(s);
}

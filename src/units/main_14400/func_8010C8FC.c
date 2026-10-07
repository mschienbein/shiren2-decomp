#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;

typedef struct { u8 pad[0xC]; u16 xC; } S;
void func_800AE6C4(S *s, u16 v);
s32 func_8010C8FC(S *s, s32 add) {
    s16 v = s->xC + add;
    if (v >= 100) {
        func_800AE6C4(s, 99);
        return v - 99;
    }
    if (v < 0) {
        func_800AE6C4(s, 0);
        return v;
    }
    func_800AE6C4(s, v);
    return 0;
}

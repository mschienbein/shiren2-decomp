#include "common.h"
typedef unsigned char u8;
typedef struct { u8 lo; u8 hi; char pad2[3]; u8 kind; char pad6[2]; } Range;
extern Range D_80156CD0[];
extern u8 D_80142F24;
extern u8 D_80142F18;
s32 func_800AA24C(void) {
    s32 i;
    i = 8;
    for (;;) {
        Range *r;
        i--;
        if (i == -1) break;
        r = &D_80156CD0[i];
        if (r->kind != D_80142F24) continue;
        if (r->lo > D_80142F18) continue;
        if (D_80142F18 > r->hi) continue;
        return i + 12;
    }
    return 0;
}

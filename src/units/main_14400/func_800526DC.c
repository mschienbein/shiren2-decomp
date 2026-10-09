#include "common.h"

typedef unsigned char u8;
extern u8 D_8016166E;
extern void func_80053158(u8 a, u8 b);

void func_800526DC(u8 a, u8 b) {
    if (D_8016166E == 0) {
        func_80053158(a, b);
    }
}

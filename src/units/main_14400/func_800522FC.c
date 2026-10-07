#include "common.h"

extern short D_80161644;
extern short D_8016166A;
extern unsigned char D_8016166D;
extern s32 func_80052D08(short id);
extern void func_80051D14(short id);
void func_800522FC(short a) {
    short *cur = &D_80161644;
    if (func_80052D08(*cur) == 0) D_8016166A = *cur;
    func_80051D14(a);
    D_8016166D = 1;
}

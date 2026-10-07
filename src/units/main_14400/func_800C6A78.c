#include "common.h"

extern signed char D_80140164;
extern signed char D_80140166;
extern s32 D_80147670;
extern s32 D_80147678;
extern void func_800C6B70(void);
extern void func_800C6F48(void);
extern void func_800C701C(void);
extern void func_800C75F4(void);

s32 func_800C6A78(void) {
    D_80147678 = 0;
    for (;;) {
        s32 active;
        s32 quit;
        func_800C6B70();
        func_800C6F48();
        func_800C701C();
        quit = 0;
        active = D_80140164 == 1;
        if (active) {
            s32 mode = D_80147670 == 3;
            if (mode || D_80140166 == 1) {
                quit = 1;
            }
        }
        if (quit) {
            D_80147678 = 12;
        }
        if (D_80147678 >= 12) {
            break;
        }
        func_800C75F4();
    }
    return D_80147678;
}

#include "common.h"

/* D_80140160: the global menu-system object (constructed by func_80092600; at least 0x62 bytes,
 * func_80092638 stores +0x61), extent not modeled here. +4 is its signed display-mode byte and +6
 * its signed quit-request byte; the original addresses them through the interior splat labels
 * D_80140164 and D_80140166. */
extern signed char D_80140160[];
static inline s32 menu_quit_requested(const signed char *menu) { return menu[6]; }
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
        active = D_80140160[4] == 1;
        if (active) {
            s32 mode = D_80147670 == 3;
            if (mode || menu_quit_requested(D_80140160) == 1) {
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

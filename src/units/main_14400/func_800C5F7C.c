#include "common.h"

extern s32 D_80140254;
extern void *D_801476B8;
extern s32 D_8014766C;
extern s32 D_80147670;
s32 D_80147674 = 0;
extern signed char D_80140160[];
extern char D_80147680[];
void *func_800A8BF0(void);
s32 func_80045D1C(void);
void func_80045CE4(s32);
void func_800C6B38(void);
void func_800C6120(void);
s32 func_800C6A78(void);
void func_800C6214(void);
s32 func_800C62B8(void);
void func_800C6874(void);
void func_800CA844(void *);
/* ODD_C: The byte-mode setter keeps the menu state conversion within its boundary. */
static inline void set_mode(s32 mode) { D_80140160[6] = mode; }
void func_800C5F7C(void) {
    D_80140254 = -1;
    D_801476B8 = func_800A8BF0();
    if (func_80045D1C() == 1) {
        func_80045CE4(1);
    } else {
        func_80045CE4(0);
    }
    D_80147670 = 1;
    for (;;) {
        s32 result;
        if (D_80147674 != 0 && D_80147670 != D_80147674) {
            D_8014766C = D_80147670;
            D_80147670 = D_80147674;
            D_80147674 = 0;
        }
        switch (D_80147670) {
        case 1:
            func_800C6B38();
            func_800C6120();
            result = func_800C6A78();
            func_800C6B38();
            if (result == 15) {
                D_80147674 = 4;
                break;
            }
            /* fall through */
        case 3:
            func_800C6214();
            D_80147674 = 4;
            break;
        case 4:
            D_80147674 = func_800C62B8();
            break;
        case 5:
            func_800C6874();
            result = func_800C6A78();
            func_800C6B38();
            if (result == 14) {
                set_mode(2);
                D_80147674 = 5;
                func_800CA844(D_80147680);
            } else if (result == 13) {
                D_80147674 = 1;
            } else {
                D_80147674 = 4;
            }
            break;
        }
    }
}

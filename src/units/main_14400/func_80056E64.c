#include "common.h"
typedef unsigned char u8;
extern s32 D_801630D0, D_80163104, D_8013A298, D_8013A290;
extern void *D_80163100;
extern char D_801630D8[], D_8014C100[], D_8013A29C[];
extern s32 func_8006E908(void *, s32, s32, s32), func_800718CC(u32, void *, void *), func_80054D50(void);
extern void *func_8006A8D8(char *name, u32 size);
extern u8 *func_8006A810(void *dst, s32 value, s32 count);
extern void func_80054DE0(s32);
extern s32 func_80054DF0(u32 mode);
s32 func_80056E64(s32 a) {
    s32 result;
    /* ODD_C: early-exit group; a bad mode or failed allocation breaks out with result -1. Also shapes
     * scheduling: nested if/else and goto-done forms are 344 vs 340 bytes, 68 words differ. */
    do {
    if ((u32)a >= 4) { result = -1; break; }
    D_801630D0 = a;
    result = func_8006E908(D_801630D8, 0x1000, 0, 0x80);
    if (result) return result;
    D_80163100 = func_8006A8D8(D_8014C100, 0x2800);
    if (!D_80163100) { result = -1; break; }
    func_8006A810(D_80163100, 0, 0x2800);
    result = func_800718CC(0, 0, D_8013A29C);
    if (result) return result;
    result = func_80054D50();
    if (!result) {
        switch (D_801630D0) {
        case 1: func_80054DE0(1); func_80054DF0(6); break;
        case 2: func_80054DE0(1); func_80054DF0(7); break;
        case 3: func_80054DE0(1); func_80054DF0(D_8013A298 + 11); D_80163104 = 0; break;
        }
        D_8013A290 = 1;
    }
    } while (0);
    return result;
}

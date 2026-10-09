#include "common.h"
extern unsigned char D_801A70E4, D_801A70E5, D_801A70E6;
extern s32 D_801A70E8, D_801A70EC;
extern short D_801A70F0;
extern void func_80074078(s32 arg);
extern void func_80074090(s32 arg);
extern void func_8007409C(s32 arg);
extern void func_80058D50(void);
void func_8006D34C(void) {
    D_801A70E4 = 1;
    D_801A70E5 = 1;
    D_801A70E8 = 0;
    D_801A70EC = -1;
    D_801A70F0 = 0;
    D_801A70E6 = 0;
    func_80074078(0);
    func_80074090(0);
    func_8007409C(0);
    func_80058D50();
}

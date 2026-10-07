#include "common.h"

extern unsigned short D_801F5D0E;
extern s32 func_80072D54(void);
extern s32 func_800610A8(void);
extern s32 func_801F284C(void);
extern void func_8008BD70(s32 count, s32 value);

void func_80046A94(s32 count, s32 value) {
    if (func_80072D54()) {
        return;
    }
    if (func_800610A8()) {
        s32 failed;
        if (D_801F5D0E) {
            return;
        }
        failed = func_801F284C() != 1;
        if (!failed) {
            return;
        }
    }
    func_8008BD70(count, value);
}

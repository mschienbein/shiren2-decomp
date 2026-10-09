#include "common.h"
extern s32 func_80046124(void);
extern void func_80045904(s32 a);
extern void func_80051D14(short id);
void func_800458BC(s32 arg) {
    if (arg == 0x44) {
        arg = func_80046124();
        func_80045904(arg);
    }
    func_80051D14(arg);
}

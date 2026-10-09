#include "common.h"

extern s32 D_80138BF0;

void func_80045A24(s32 sound);
void func_80060C54(u32 mode);
void func_800B2B50(void);
void func_80046374(void);

/* The original direct call supplies its stack object; this method uses globals. */
void func_800480EC(void *unused_receiver) {
    func_80045A24(3);
    func_80060C54(3);
    func_800B2B50();
    D_80138BF0 = 0;
    func_80046374();
}

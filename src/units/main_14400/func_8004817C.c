#include "common.h"

extern void func_80060C54(u32 mode);
extern void func_800B2B50(void);
extern void func_80046374(void);
extern s32 D_80138BF0;

/* ROM caller 0x1426C0 supplies D_80140100; incoming a0 is overwritten
 * at VRAM 0x80048188 before use. Preserve the supplied receiver contract. */
void func_8004817C(void *unused_receiver) {
    func_80060C54(3);
    func_800B2B50();
    D_80138BF0 = 0;
    func_80046374();
}

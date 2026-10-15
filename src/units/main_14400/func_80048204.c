#include "common.h"
extern void func_80045A24(s32 sound);
extern void func_80060C54(u32 mode);
extern void func_800B2B50(void);
extern void func_80046374(void);
extern s32 D_80138BF0;
/* ROM caller 0x142EE0 supplies D_80140130; incoming a0 is overwritten
 * at VRAM 0x80048210 before use. Preserve the supplied receiver contract. */
void func_80048204(void *unused_receiver)
{
    func_80045A24(3);
    func_80060C54(3);
    func_800B2B50();
    D_80138BF0 = 0;
    func_80046374();
}

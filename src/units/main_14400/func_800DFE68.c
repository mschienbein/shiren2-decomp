#include "common.h"
extern void func_800498E4(s32, ...);
extern s32 func_80049CB4(s32, ...);
/* Virtual action slot +0x14 supplies an unused receiver. */
s32 func_800DFE68(void *unused_receiver) {
    func_800498E4(0x220);
    func_80049CB4(0x129, 0x3c);
    func_80049CB4(2);
    return 6;
}

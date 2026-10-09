#include "common.h"

typedef unsigned short u16;
extern void func_80060C54(u32 mode);
extern void func_80062804(u32 mode, s32 arg1, s32 arg2, s32 arg3);
extern void func_80066B2C(u16 *cells, s32 room);
extern void func_80058FD0(void);
extern void func_80062920(void);

void func_80056DA0(void) {
    func_80060C54(4);
    func_80062804(1, 0x11, 0, 0);
    func_80066B2C(0, -1);
    func_80058FD0();
    func_80062920();
}

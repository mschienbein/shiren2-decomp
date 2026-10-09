#include "common.h"
extern void func_80058FD0(void);
extern void func_80062920(void);
extern s32 func_800610BC(void);
extern void func_800590BC(void);
void func_8005EF9C(void)
{
    func_80058FD0();
    func_80062920();
    if (func_800610BC() == 10) {
        func_800590BC();
    }
}

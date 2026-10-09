#include "common.h"
typedef unsigned char u8;
extern u8 func_80131E54(void);
extern u8 func_80130A80(void);
extern void func_801312E0(void);
extern void func_801319A0(void);
u8 func_80130A40(void)
{
    u8 result = func_80131E54();
    func_80130A80();
    func_801312E0();
    func_801319A0();
    return result;
}

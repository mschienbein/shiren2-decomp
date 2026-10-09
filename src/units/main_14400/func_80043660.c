#include "common.h"

extern void func_80043BB8(unsigned long devAddr, unsigned long value);
extern void func_80043768(void);

void func_80043660(void)
{
    func_80043BB8(0x08007FF0, 0x12345678);
    func_80043768();
}

#include "common.h"
extern void func_80043C2C(u32 deviceAddress, void *destination, u32 length);
extern void func_80043D74(u32 deviceAddress, void *source, u32 length);
void func_80042BA8(u32 deviceAddress, void *buffer, u32 length, s32 read)
{
    if (read != 0) {
        func_80043C2C(deviceAddress, buffer, length);
    } else {
        func_80043D74(deviceAddress, buffer, length);
    }
}

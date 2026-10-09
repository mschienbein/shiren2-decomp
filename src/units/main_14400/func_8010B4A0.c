#include "common.h"
typedef unsigned char u8;
char *func_800A3B20(void *object);
void func_800498E4(s32 id, ...);
s32 func_800EE8E8(void *object, s32 mode, s32 key, u8 value, s32 extra);
s32 func_8010B4A0(void *object, s32 mode, s32 key, u8 value, s32 extra)
{
    if (mode == 0 && key == 8) {
        func_800498E4(0x224, func_800A3B20(object));
        return 1;
    }
    return func_800EE8E8(object, mode, key, value, extra);
}

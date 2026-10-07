#include "common.h"

typedef unsigned char u8;

extern s32 func_8010BEC4(void *obj, u8 effect);
extern s32 func_8010BF0C(void *obj);

void func_8010CDCC(void *obj, u32 *flags) {
    s32 guarded = (u8)func_8010BEC4(obj, 0x68) || (u8)func_8010BEC4(obj, 0xF);

    if (guarded) {
        *flags |= 0x100;
    }
    if (func_8010BF0C(obj)) {
        *flags |= 0x200;
    }
    if ((u8)func_8010BEC4(obj, 0x60)) {
        *flags |= 0x400;
    }
    if ((u8)func_8010BEC4(obj, 0x67)) {
        *flags |= 0x2000;
    }
    if ((u8)func_8010BEC4(obj, 0x66)) {
        *flags |= 0x1000;
    }
    if ((u8)func_8010BEC4(obj, 0x59)) {
        *flags |= 0x8000;
    }
    if ((u8)func_8010BEC4(obj, 0xF)) {
        *flags |= 0x100;
    }
}

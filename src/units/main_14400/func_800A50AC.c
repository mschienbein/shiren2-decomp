#include "common.h"

extern char D_80147620[];
extern unsigned char func_800C57CC(void *, s32);
extern s32 func_800A5018(void *, s32);
s32 func_800A50AC(void *object) {
    return func_800A5018(object, func_800C57CC(D_80147620, 3) & 0xff);
}

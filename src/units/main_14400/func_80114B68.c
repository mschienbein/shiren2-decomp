#include "common.h"
typedef unsigned short u16;
extern unsigned short D_80157740[];
extern unsigned char D_80157788[];
unsigned char func_800AE98C(void *a);
s32 func_800AC584(u16 v);
u32 func_80114B68(void *a) {
    unsigned char k = func_800AE98C(a);
    s32 v = func_800AC584(D_80157740[k]);
    return (u32)v * D_80157788[k] / 100;
}

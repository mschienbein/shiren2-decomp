#include "common.h"
typedef unsigned char u8;
extern s32 D_80165394;
extern u8 D_8013B140[];
void func_800593F8(void *buf);
void func_800594C0(void *buf, s32 id);
void func_80059590(s32 id) {
    u8 buf[16];
    if (id != D_8013B140[D_80165394]) {
        func_800593F8(buf);
        func_800594C0(buf, id);
    }
}

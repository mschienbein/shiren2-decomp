#include "common.h"
typedef unsigned char u8;
extern s32 func_800B3FE0(u8 *a, u8 *b);
extern void func_800B4098(u8 a, u8 b);
extern void *func_800C5F60(void);
extern s32 func_80049CB4(s32 id, ...);
extern void func_800E2298(void *obj);
s32 func_800B4344(void) {
    u8 a, b;
    s32 changed = 0;
    while (func_800B3FE0(&a, &b)) {
        changed = 1;
        func_800B4098(a, b);
    }
    if (changed) {
        void *obj = func_800C5F60();
        func_80049CB4(0xA9, obj);
        func_800E2298(obj);
    }
    return 1;
}

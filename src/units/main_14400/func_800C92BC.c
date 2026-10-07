#include "common.h"
extern s32 D_80147664;
extern s32 D_80147668;
void func_800E2298(void *obj);
s32 func_80049CB4(s32 id, ...);
void func_800C92BC(void *obj) {
    func_800E2298(obj);
    func_80049CB4(0xDB);
    func_80049CB4(2);
    D_80147664 = 0;
    D_80147668 = 0;
}

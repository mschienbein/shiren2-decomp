#include "common.h"

s32 func_800E4B60(void *obj, void *event);
s32 func_800A99D0(void);
s32 func_800E9E88(void *obj);
void func_800E9FDC(void *obj);

void func_800EA784(void *obj, void *event) {
    func_800E4B60(obj, event);
    if (func_800A99D0() == 0) {
        s32 blocked = func_800E9E88(obj) == 1;

        if (!blocked) {
            func_800E9FDC(obj);
        }
    }
}

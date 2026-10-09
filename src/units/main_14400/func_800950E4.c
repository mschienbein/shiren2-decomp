#include "common.h"
extern s32 func_8006D44C(s32 mode, s32 timeout);
extern void func_80048728(void *object);
void func_800950E4(unsigned char *object, s32 timeout) {
    func_8006D44C(2, timeout);
    func_80048728(object + 4);
}

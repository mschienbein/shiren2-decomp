#include "common.h"
extern void func_800E075C(void *);
extern s32 func_800E0C2C(void *);
extern s32 func_800EAFB8(void *, s32);
s32 func_800EAF38(void *object, s32 notify) {
    func_800E075C(object);
    func_800E0C2C(object);
    return func_800EAFB8(object, notify);
}

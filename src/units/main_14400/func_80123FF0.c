#include "common.h"

extern s32 D_80153AA0[];
extern void func_800AC68C(void *);

void func_80123FF0(void **object, s32 flags) {
    object[2] = D_80153AA0;
    if (flags & 1) {
        func_800AC68C(object);
    }
}

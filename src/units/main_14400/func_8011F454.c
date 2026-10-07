#include "common.h"

typedef unsigned char u8;

extern u8 D_80153AA0[];
void func_800AC68C(void *);
void func_8011F454(u8 *self, s32 flags) {
    *(void **)(self + 8) = D_80153AA0;
    if (flags & 1) {
        func_800AC68C(self);
    }
}

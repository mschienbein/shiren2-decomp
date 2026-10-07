#include "common.h"
extern s32 D_80153AA0[];
void func_800AC68C(void *);
void func_8012005C(void *self, s32 flags) {
    ((void **)self)[2] = D_80153AA0;
    if (flags & 1) {
        func_800AC68C(self);
    }
}

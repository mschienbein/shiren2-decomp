#include "common.h"

extern unsigned char D_80153AA0[];
void func_800AC68C(void *);

void func_8011E8F4(void **obj, s32 flags)
{
    obj[2] = D_80153AA0;
    if (flags & 1) {
        func_800AC68C(obj);
    }
}

#include "common.h"
typedef unsigned char u8;
typedef unsigned short u16;
extern void *D_801476B8;
extern u16 D_80156916;
extern u32 func_8011575C(void *owner);
extern s32 func_800E0F40(void *object);
u16 func_80115944(void *owner, u16 value) {
    if (func_8011575C(owner)) {
        u16 bonus = D_80156916 * ((u8)func_800E0F40(D_801476B8) - 1);
        value = value * (bonus + 100) / 100;
    }
    return value;
}

#include "common.h"

typedef unsigned char u8;

typedef struct {
    s32 index;
    void *owner;
    s32 unk8;
    s32 unkC;
} Iter80119A40;

Iter80119A40 *func_800CEB20(Iter80119A40 *it, void *collection);
s32 func_800CEBA0(Iter80119A40 *it);
void *func_800CEC68(Iter80119A40 *it);
s32 func_801199E0(void *arg0, void *item, u8 arg2, u8 arg3);
void func_800CD3D0(void *list, u32 index);

s32 func_80119A40(void *arg0, void *arg1, u8 arg2, u8 arg3) {
    Iter80119A40 it;
    s32 found = 0;

    func_800CEB20(&it, arg1);
    while (func_800CEBA0(&it)) {
        if (func_801199E0(arg0, func_800CEC68(&it), arg2, arg3)) {
            do {
                func_800CD3D0(arg1, it.index + 1);
                found = 1;
            } while (0);
        }
    }
    return found;
}

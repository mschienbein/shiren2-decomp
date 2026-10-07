#include "common.h"
typedef unsigned char u8;
typedef signed char s8;
typedef struct { s32 unk[4]; } Iter;
Iter *func_800CEB20(Iter *, void *);
s32 func_800CEBA0(Iter *);
u8 *func_800CEC68(Iter *);
void func_800AE974(u8 *, s8);
void *func_8011422C(u8 *);
void func_800CDC2C(void *node, s8 arg) {
    Iter it;
    func_800CEB20(&it, node);
    while (func_800CEBA0(&it)) {
        u8 *n = func_800CEC68(&it);
        func_800AE974(n, arg);
        if (*n == 9) func_800CDC2C(func_8011422C(n), arg);
    }
}

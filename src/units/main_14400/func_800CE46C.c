#include "common.h"

typedef signed char s8;
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;

typedef struct { s32 unk[4]; } Iter;
Iter *func_800CEB20(Iter *, void *);
s32 func_800CEBA0(Iter *);
void *func_800CEC68(Iter *);
s32 func_800CE46C(void *list, s32 (*cb)(void *)) {
    Iter it;
    func_800CEB20(&it, list);
    while (func_800CEBA0(&it)) {
        if (cb(func_800CEC68(&it))) {
            return 1;
        }
    }
    return 0;
}

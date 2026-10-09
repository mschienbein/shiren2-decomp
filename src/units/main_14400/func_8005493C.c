#include "common.h"

typedef unsigned short u16;

extern u16 D_80162B46;
extern u16 D_80162B48;

void func_80053AE8(s32 index);

void func_8005493C(void) {
    D_80162B46 = D_80162B48;
    func_80053AE8(0);
}

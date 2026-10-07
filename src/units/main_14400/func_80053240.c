#include "common.h"

typedef unsigned char u8;

typedef struct {
    s32 field_0;
    s32 field_4;
} Pair80053240;

extern u8 D_801398A0;
extern Pair80053240 D_80161700;
extern Pair80053240 D_80161708;

void func_80053100(void);

void func_80053240(u8 divisor) {
    if (divisor == 0) {
        func_80053100();
        return;
    }
    D_801398A0 = 1;
    D_80161700.field_0 = 0;
    D_80161708.field_0 = 0;
    D_80161700.field_4 = 0x4FFF / divisor;
    D_80161708.field_4 = 0x67FF / divisor;
}

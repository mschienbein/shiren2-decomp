#include "common.h"

typedef unsigned char u8;

extern u8 D_80142F24;
extern u8 D_80142F25;
extern u8 D_80148780[3];
extern u8 func_800A9958(void);

void func_8012256C(void) {
    u8 *out = D_80148780;

    out[0] = D_80142F24;
    out[1] = func_800A9958();
    out[2] = D_80142F25;
}

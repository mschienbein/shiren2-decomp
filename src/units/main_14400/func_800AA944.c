#include "common.h"

extern unsigned char D_801476D1;
extern unsigned char D_80142F1B;
extern signed char D_801476C2;
extern unsigned char D_80142F20;
void func_80045A60(void);
void func_80045A84(void);
void func_80045E3C(void);

void func_800AA944(void) {
    s32 idle;

    if (D_801476D1 != 0) {
        return;
    }
    idle = ((D_80142F1B >> 2) & 1) ^ 1;
    if (idle) {
        if (D_801476C2 == 0) {
            func_80045A60();
        }
        return;
    }
    switch (D_80142F20) {
    case 0x61:
    case 0x67:
    case 0x6A:
    case 0x6D:
        func_80045E3C();
        func_80045A60();
        break;
    default:
        func_80045A84();
        break;
    }
}

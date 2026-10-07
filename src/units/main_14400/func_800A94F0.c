#include "common.h"
typedef struct { unsigned char f0, f1, f2, f3, f4, f5, f6, f7, f8; signed char f9; } S42F24;
extern S42F24 D_80142F24;
void func_800A94F0(void) {
    S42F24 *p = &D_80142F24;
    p->f3 = 1;
    p->f0 = 0;
    p->f1 = 0;
    p->f2 = 0;
    p->f4 = 0;
    p->f5 = 0;
    p->f9 = -1;
}

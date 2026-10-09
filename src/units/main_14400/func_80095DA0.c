#include "common.h"

typedef struct Object Object;

extern Object *D_80140234[8];
extern s32 D_80140254;

void func_80095D20(Object *arg);

void func_80095DA0(void)
{
    s32 i;

    for (i = 0; i < D_80140254; i++) {
        func_80095D20(D_80140234[i]);
    }
}

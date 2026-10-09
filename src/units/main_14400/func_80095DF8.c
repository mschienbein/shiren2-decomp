#include "common.h"

typedef struct Obj Obj;
extern s32 D_80140254;
extern Obj *D_80140234[8];
extern void func_80046E7C(Obj *obj);
void func_80095DF8(void)
{
    s32 i;
    for (i = 0; i < D_80140254; i++) {
        func_80046E7C(D_80140234[i]);
    }
}

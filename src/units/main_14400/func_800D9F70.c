#include "common.h"
typedef struct { char pad[0x104]; void *x104; } S;
extern S *D_801476B8;
s32 func_800E4454(S *p);
void func_800E4470(S *p);
void func_800EBCD0(S *p);
s32 func_800D9F70(void *self /* receiver: unused; supplied by the vtable +0x14 call */) {
    if (func_800E4454(D_801476B8)) {
        func_800E4470(D_801476B8);
        return 0;
    }
    if (D_801476B8->x104) func_800EBCD0(D_801476B8);
    return 1;
}

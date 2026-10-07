#include "common.h"
typedef struct { s32 a; s32 b; } Pair;
extern Pair *D_801476B8;
extern s32 D_80148260;
void *func_800E215C(void *obj);
s32 func_80049CB4(s32 id, ...);
void func_800DAD20(void *obj, void *target) {
    if (target == func_800E215C(D_801476B8)) {
        Pair tmp;
        Pair *t = &tmp;
        t->a = D_801476B8->a;
        t->b = D_801476B8->b;
        func_80049CB4(0xE1, t);
        D_80148260 = 1;
    }
}

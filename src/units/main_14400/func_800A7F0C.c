#include "common.h"
typedef struct { s32 a; s32 b; } Pair800A7F0C;
void *func_800A6538(void *out_direction, void *obj, void *target);
void func_800A665C(void *, Pair800A7F0C *);
void func_800A7F0C(void *self, void *target) {
    Pair800A7F0C tmp;
    Pair800A7F0C *t = &tmp;
    func_800A6538(t, self, target);
    func_800A665C(self, t);
}

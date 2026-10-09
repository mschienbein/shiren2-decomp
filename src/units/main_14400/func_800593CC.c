#include "common.h"

extern s32 D_80165394;
unsigned char D_8013B140[5] = {1, 2, 0, 0, 1};
extern void func_800594C0(void *pos, s32 mode);

void func_800593CC(void *self)
{
    func_800594C0(self, D_8013B140[D_80165394]);
}

#include "func_801E7A28_139948/types.h"

extern Rec801F6764 D_801F6764_148684[30];
extern void func_8007935C(s32 i);
extern void func_800265E0(void *dst, s32 length);
extern void func_801E6BA0_138AC0(s32 idx);

void func_801E7A28_139948(s32 idx) {
    if (D_801F6764_148684[idx].x14 != 0) {
        func_8007935C(D_801F6764_148684[idx].x14 - 1);
    }
    func_800265E0(&D_801F6764_148684[idx], sizeof(Rec801F6764));
    func_801E6BA0_138AC0(idx);
}

#include "func_801E75CC_1394EC/types.h"

extern Rec801F6764 D_801F6764_148684[30];
extern Unit *func_8007946C(s32 side, s32 slot);

void func_801E75CC_1394EC(s32 i) {
    D_801F6764_148684[i].x17 = 1;
    func_8007946C(0, i)->x7 = 2;
}

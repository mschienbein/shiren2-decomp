#include "row_views.h"

extern u16 D_8013E810;
extern s32 D_8013E814;
extern s32 D_8013E818;
extern s32 D_8013E8E0;
extern u16 D_801A9058[];
void func_80081798(void);
void func_80081F88(void) {
    s32 i;
    for (i = D_8013E814; i < D_8013E818; i++) {
        D_801A9080[D_801A9058[i]].field_02 = D_8013E810;
    }
    if (D_8013E818 > D_8013E814) {
        func_80081798();
        D_8013E8E0 = 1;
    }
    if (++D_8013E810 >= 0x40) {
        D_8013E810 = 0x3F;
    }
}

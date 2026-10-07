#include "row_views.h"
extern s32 D_8013E814;
extern s32 D_8013E818;
extern s32 D_8013E8DC;
extern unsigned short D_801A9058[];
void func_80082150(void) {
    s32 i;
    for (i = D_8013E814; i < D_8013E818; i++) {
        D_801A9080[D_801A9058[i]].field_02 = 0x41;
    }
    if (D_8013E818 > D_8013E814) {
        D_8013E8DC = 1;
    }
}

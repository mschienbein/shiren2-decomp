#include "row_views.h"

extern s32 D_8013E8E0;
extern s32 D_8013E818;
s32 D_8013E814 = 0;
void func_80081EFC(void) {
    s32 i;
    for (i = 9; i >= 0; i--) {
        D_801A9080[i].field_02 = 0;
    }
    D_8013E8E0 = 1;
    D_8013E818 = 0;
    D_8013E814 = 0;
}

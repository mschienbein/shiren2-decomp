#include "row_views.h"

void func_800826FC(s32 index)
{
    if (index != D_8013E820) {
        D_8013E820 = -1;
        D_8013E81C = 0;
        if ((u32)index < 10 && D_801A9080[index].field_02 != 0) {
            D_8013E81C = &D_801A9080[index];
            D_801A9F5A = 12;
            D_801A9F5E = 12;
            D_8013E820 = index;
            D_801A9F58 = 0;
            D_801A9F5C = 0;
            D_8013E834 = 6;
            D_8013E838 = 0;
            func_800827C8(15);
        }
    }
}

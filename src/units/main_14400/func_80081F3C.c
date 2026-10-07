#include "row_views.h"

extern s32 D_8013E8DC;

void func_80081F3C(u32 index, u16 value) {
    if (index < 10) {
        if (D_801A9080[index].field_02 != 0) {
            D_801A9080[index].field_02 = value;
            D_8013E8DC = 1;
        }
    }
}

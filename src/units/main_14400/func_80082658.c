#include "row_views.h"

void func_80082658(u32 index, s32 value) {
    if (index < 10 && D_801A9080[index].field_02 != 0) {
        D_801A9080[index].field_1A = value;
    }
}

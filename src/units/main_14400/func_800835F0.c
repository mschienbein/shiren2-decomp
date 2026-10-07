#include "row_views.h"

void func_800835F0(u32 arg0, u32 arg1, u32 arg2, u32 arg3) {
    if (arg0 < 10U && D_801A9080[arg0].field_02 != 0) {
        D_801A9080[arg0].field_14 = arg1;
        D_801A9080[arg0].field_16 = arg2;
        D_801A9080[arg0].field_18 = arg3;
    }
}

#include "row_views.h"

extern s32 D_8013E818;
void func_800823D8(s32 id, s32 v);
void func_80082600(s32 id) {
    if (D_801A9080[id].field_0E != 0) func_800823D8(id, D_8013E818 - D_801A9080[id].field_0E);
}

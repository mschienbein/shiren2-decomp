#include "row_views.h"

extern s32 D_8013E818;
extern void func_800823D8(s32, s32);
void func_800825AC(s32 i) {
    s32 n = D_8013E818;
    s32 v = D_801A9080[i].field_0E;
    if (v < n - 1) func_800823D8(i, n - 2 - v);
}

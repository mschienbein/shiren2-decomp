#include "common.h"

extern void func_8007D8C8(void);
extern s32 D_8013DFD0;
extern s32 D_801A8070;

void func_8007D84C(s32 arg0) {
    s32 temp_v0;

    temp_v0 = D_801A8070;
    D_801A8070 = arg0;
    if (temp_v0 != arg0) {
        if (arg0 != 0) {
            D_8013DFD0 = -1;
        }
        func_8007D8C8();
    }
}

#include "func_801E6380_134ED0/types.h"

extern s32 D_801E7890_1363E0;
extern s32 D_801E7894_1363E4;
extern void func_801E63C4_134F14(void);
extern s32 func_801E5108_133C58(void);

s32 func_801E6380_134ED0(void) {
    s32 result;
    if (D_801E7890_1363E0 != 0) {
        func_801E63C4_134F14();
    }
    result = func_801E5108_133C58();
    D_801E7894_1363E4 = result;
    D_801E7890_1363E0 = 1;
    return result;
}

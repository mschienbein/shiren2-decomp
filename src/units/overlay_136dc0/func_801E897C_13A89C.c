#include "func_801E897C_13A89C/types.h"
extern s32 func_8005B058(void);
extern void func_8005A950(s32 dir, float value, s32 mode);
s32 func_801E897C_13A89C(s32 dir, float value) {
    s32 state = func_8005B058();
    func_8005A950(dir, value, 4);
    return state >= 3;
}

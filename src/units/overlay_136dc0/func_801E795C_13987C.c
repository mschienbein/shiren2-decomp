#include "func_801E795C_13987C/types.h"

extern u8 D_801F6EA8_148DC8;
extern void func_8005C870(u8 v);
extern void func_800559F8(s32 value);
extern void func_80055874(void);
extern s32 func_80041E3C(void);
extern void func_80076714(s32 on);

void func_801E795C_13987C(void) {
    s32 t;

    if (D_801F6EA8_148DC8 != 0) {
        func_8005C870(2);
        t = ++D_801F6EA8_148DC8 - 2;
        if (t == 0) {
            func_800559F8(1);
        } else if (t == 40) {
            func_800559F8(3);
        } else if (t >= 48) {
            func_80055874();
            D_801F6EA8_148DC8 = 0;
        }
        if ((u8)func_80041E3C() == 1) {
            func_80076714(1);
        } else {
            func_80076714(0);
        }
    }
}

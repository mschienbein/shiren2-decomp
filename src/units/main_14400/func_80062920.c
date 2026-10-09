#include "common.h"

extern u32 D_8016DB18;
extern void func_8006179C(void);
extern void func_80061DD0(s32 x0, s32 y0, s32 x1, s32 y1);
extern void func_8006599C(void);
extern void func_800688E8(void);
extern void func_80069BF4(void);

void func_80062920(void)
{
    func_8006179C();
    func_80061DD0(10, 10, 65, 43);
    switch (D_8016DB18) {
        case 1:
        case 2: func_8006599C(); break;
        case 3: func_800688E8(); break;
        case 4: func_80069BF4(); break;
    }
}

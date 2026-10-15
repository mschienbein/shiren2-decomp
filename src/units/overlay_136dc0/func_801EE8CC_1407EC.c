#include "func_801EE8CC_1407EC/types.h"

extern D_801F51EC_s *D_801F51EC_14710C;
s32 func_801EC568_13E488(void);
void func_801EC7E8_13E708(void);
u8 func_801E53A4_1372C4(s32);
void func_801E8F20_13AE40(s32);

void func_801EE8CC_1407EC(s32 arg0) {
    if (D_801F51E8_147108->b997 != 0) {
        s32 state = D_801F51E8_147108->b997;

        switch (state) {
        case 1:
            if (D_801F51E8_147108->b991 == 0) {
                if (arg0 != 0) {
                    if (func_801E53A4_1372C4(D_801F51EC_14710C->x4) & 0x80) {
                        D_801F51E8_147108->b991 = state;
                        break;
                    }
                } else {
                    D_801F51E8_147108->b991 = state;
                    break;
                }
            }
            D_801F51E8_147108->b991 = 0;
            func_801EC7E8_13E708();
            break;
        case 2:
            func_801EC7E8_13E708();
            break;
        case 3:
        case 4:
            func_801E8F20_13AE40(1);
            func_801EC7E8_13E708();
            break;
        }
    } else {
        func_801EC568_13E488();
    }
}

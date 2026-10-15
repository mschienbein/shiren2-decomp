#include "func_801EE9A8_1408C8/types.h"
extern S10EventState *D_801F51E8_147108;
extern s32 func_801E51CC_1370EC(void);
extern u8 func_801E9F70_13BE90(s32 flag);

s32 func_801EE9A8_1408C8(void)
{
    s32 state;
    if (D_801F51E8_147108->field120 != 0 &&
        ((state = func_801E51CC_1370EC()) == 0 ||
         (state != -1 && (D_801F51E8_147108->field120 & 0x8000)))) {
        if (func_801E9F70_13BE90(0xFD)) {
            D_801F51E8_147108->field9FC = 1;
        }
        return D_801F51E8_147108->field120 & 0x7FFF;
    }
    return 0;
}

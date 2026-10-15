#include "func_801EB444_13D364/types.h"
extern void func_801E5D6C_137C8C(void);
extern void func_801F33FC_14531C(void);

s32 func_801EB444_13D364(s32 frames)
{
    /* ROM 0x13D398..0x13D3B4: this function owns the four-byte countdown
     * at BSS VRAM 0x801F6F48. No other original instruction references it. */
    static s32 D_801F6F48_148E68;
    func_801E5D6C_137C8C();
    func_801F33FC_14531C();
    if (!D_801F51E8_147108->field994) {
        D_801F51E8_147108->field994 = 1;
        D_801F6F48_148E68 = frames;
    }
    if (D_801F6F48_148E68 == 0) {
        D_801F51E8_147108->field994 = 0;
        return 0;
    }
    D_801F6F48_148E68--;
    return 1;
}

#include "func_801EBC44_13DB64/types.h"

s32 func_801EBC44_13DB64(void)
{
    if (D_801F51E8_147108->field9F4 &&
        D_801F51E8_147108->field2A6[D_801F51E8_147108->field9F4 - 1] == D_801F5CCD_147BED) {
        D_801F51E8_147108->field9F4--;
        return 0;
    }
    D_801F51E8_147108->field2A6[D_801F51E8_147108->field9F4++] = D_801F5CCD_147BED;
    return 1;
}

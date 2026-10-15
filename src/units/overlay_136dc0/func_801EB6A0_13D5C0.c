#include "func_801EB6A0_13D5C0/types.h"

void func_801EB6A0_13D5C0(u8 index)
{
    switch (D_801F51E8_147108->field122[index]) {
    case 1:
        D_801F51E8_147108->field108[index].word |= ~0xFF;
        break;
    case 2:
        D_801F51E8_147108->field108[index].word |= ~0xFFFF;
        break;
    case 3:
        D_801F51E8_147108->field108[index].word |= ~0xFFFFFF;
        break;
    }
}

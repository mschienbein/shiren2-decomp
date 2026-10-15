#include "func_801EB954_13D874/types.h"

void func_801EB954_13D874(u8 code)
{
    s32 index = (code & 0xC) >> 2;
    D_801F51E8_147108->field122[index] = 0;
    switch (code & 3) {
    case 3:
        D_801F51E8_147108->field108[index].word = (u32)func_801E53A4_1372C4(D_801F51EC_14710C->field4++) << 24;
        D_801F51E8_147108->field108[index].word += func_801E53A4_1372C4(D_801F51EC_14710C->field4++) << 16;
        D_801F51E8_147108->field108[index].word += func_801E53A4_1372C4(D_801F51EC_14710C->field4++) << 8;
        D_801F51E8_147108->field108[index].word += func_801E53A4_1372C4(D_801F51EC_14710C->field4++);
        break;
    case 2:
        D_801F51E8_147108->field108[index].word = func_801E53A4_1372C4(D_801F51EC_14710C->field4++) << 16;
        D_801F51E8_147108->field108[index].word += func_801E53A4_1372C4(D_801F51EC_14710C->field4++) << 8;
        D_801F51E8_147108->field108[index].word += func_801E53A4_1372C4(D_801F51EC_14710C->field4++);
        if (D_801F51E8_147108->field108[index].word & 0x800000) {
            D_801F51E8_147108->field122[index] = 3;
        }
        break;
    case 1:
        D_801F51E8_147108->field108[index].word = func_801E53A4_1372C4(D_801F51EC_14710C->field4++) << 8;
        D_801F51E8_147108->field108[index].word += func_801E53A4_1372C4(D_801F51EC_14710C->field4++);
        if (D_801F51E8_147108->field108[index].word & 0x8000) {
            D_801F51E8_147108->field122[index] = 2;
        }
        break;
    case 0:
        D_801F51E8_147108->field108[index].word = func_801E53A4_1372C4(D_801F51EC_14710C->field4++);
        if (D_801F51E8_147108->field108[index].word & 0x80) {
            D_801F51E8_147108->field122[index] = 1;
        }
        break;
    }
}

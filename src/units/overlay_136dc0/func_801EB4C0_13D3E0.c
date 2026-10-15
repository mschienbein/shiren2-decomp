#include "func_801EB4C0_13D3E0/types.h"

void func_801EB4C0_13D3E0(void)
{
    u8 code = func_801E53A4_1372C4(D_801F51EC_14710C->field4++);
    if (code == 0x4E) {
        D_801F51EC_14710C->field4 += 2;
    } else if (code != 3) {
        code = (code & 3) + 2;
        D_801F51EC_14710C->field4 += code;
    }
}

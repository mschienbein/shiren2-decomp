#include "func_801EB53C_13D45C/types.h"

OvlScriptRecord *func_801EB53C_13D45C(OvlScriptRecord *record)
{
    s32 id = func_801E53A4_1372C4(record->field4++);
    id <<= 8;
    id += func_801E53A4_1372C4(record->field4++);
    if (++D_801F5CCC_147BEC >= 8) {
        D_801F5CCC_147BEC--;
    }
    return func_801EB5D4_13D4F4((u16)id);
}

#include "func_801EB5D4_13D4F4/types.h"

OvlScriptRecord *func_801EB5D4_13D4F4(u16 id)
{
    OvlScriptRecord *record = &D_801F5C8C_147BAC[D_801F5CCC_147BEC];
    func_800265E0(record, sizeof(*record));
    record->field0 = id;
    record->field4 = func_801E5470_137390(id);
    return record;
}

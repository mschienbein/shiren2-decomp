#include "func_801EB638_13D558/types.h"

/* Caller 801EBD54..801EBD60 explicitly supplies D_801F51EC_14710C.
 * Incoming a0 is unused: the first callee write is lhu a0,0(s0) at 801EB670. */
OvlScriptRecord *func_801EB638_13D558(OvlScriptRecord *unused_record)
{
    OvlScriptRecord *record;
    if (D_801F5CCC_147BEC == 0) {
        return 0;
    }
    record = &D_801F5C8C_147BAC[--D_801F5CCC_147BEC];
    func_801E5470_137390(record->field0);
    return record;
}

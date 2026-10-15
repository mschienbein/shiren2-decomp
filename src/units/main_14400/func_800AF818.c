#include "common.h"
#include "item_records.h"

/* The seven item pools (16 bytes apart); the pointer table below is owned by this unit. */
ItemPool *const D_80153AE4[7] = {
    &D_80143094, &D_801430A4, &D_801430B4, &D_801430C4,
    &D_801430D4, &D_801430E4, &D_801430F4,
};

ItemPool *func_800AF818(u32 index)
{
    if (index >= 7) {
        return 0;
    }
    return D_80153AE4[index];
}

#include "common.h"

/* Partial access view; historical record ownership and API are unresolved. */
typedef struct {
    unsigned char unknown_00[0x40];
    unsigned short field_40;
} RecordView_800E1148;

u32 func_800E1148(const RecordView_800E1148 *record)
{
    return (((u32)record->field_40 & 0x0F00u) >> 8) + 17u;
}

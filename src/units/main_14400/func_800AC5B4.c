#include "common.h"
#include "item_records.h"

/* Callers supply size; the fixed-pool choice depends only on alternate. The record is raw
 * storage for the caller's item constructor. */
void *func_800AC5B4(s32 size, s32 alternate) {
    void *result;
    if (alternate == 0) {
        result = func_800AFA7C(&D_80143094);
    } else {
        result = func_800AFAFC(&D_80143094);
    }
    return result;
}

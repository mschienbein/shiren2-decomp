#include "common.h"

/* Partial compatible word view; full owner/type and API remain unresolved. */
typedef struct {
    u32 field00;
    u32 field04;
} ObservedWords_800A28A0;

void func_800A28A0(ObservedWords_800A28A0 *record)
{
    u32 original00 = record->field00;
    u32 original04 = record->field04;

    record->field04 = original00;
    record->field00 = original04;
}

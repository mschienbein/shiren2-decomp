#include "common.h"
#include "field_view_800EC68C.h"

/* Proposed effect signature; the historical public API remains unproved. */
void func_800EC68C(Func800EC68CByteView *record, u32 value) {
    record->field_109 = (unsigned char)value;
    record->field_10A = 0;
    record->field_10B = 0;
}

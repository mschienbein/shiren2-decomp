#include "common.h"

typedef struct {
    s32 field_0;
    char pad4[4];
    s32 field_8;
} Range;

s32 func_800A315C(Range *range) {
    s32 diff = range->field_8 - range->field_0;

    if (diff < 0) {
        return diff - 1;
    }
    return diff + 1;
}

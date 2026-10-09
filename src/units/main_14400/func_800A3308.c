#include "common.h"

typedef struct { s32 field_00, field_04, field_08, field_0C; } Bounds;
extern Bounds *func_800A324C(Bounds *, void *, void *);
static inline s32 x_positive(Bounds *bounds) { return bounds->field_00 > bounds->field_08; }
static inline s32 y_positive(Bounds *bounds) { return bounds->field_04 > bounds->field_0C; }
s32 func_800A3308(void *first, void *second) {
    Bounds bounds;
    func_800A324C(&bounds, first, second);
    {
    s32 value = 0;
    if (x_positive(&bounds)) value = bounds.field_00 - bounds.field_08;
    if (y_positive(&bounds)) value += bounds.field_04 - bounds.field_0C;
    return value;
    }
}

#include "common.h"
typedef short s16;
typedef struct { unsigned char pad_00[0xC]; signed char base_0C; unsigned char bonus_0D; } Obj;
s32 func_8010BA90(Obj *object, s16 value)
{
    s16 previous = (signed char)object->bonus_0D;
    s32 belowMinimum;
    value += previous;
    if (value >= 100) {
        value = 99;
    } else {
        /* ODD_C: retain the lower-bound predicate separately from the narrow
           assignment; this also preserves the compiler's comparison lifetime. */
        belowMinimum = -object->base_0C > value;
        if (belowMinimum) {
            value = -object->base_0C;
        }
    }
    object->bonus_0D = value;
    return value != previous;
}

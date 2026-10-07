#include "common.h"

typedef struct {
    s32 field_0;
    s32 start;
    s32 field_8;
    s32 end;
} Span;

s32 func_800A3138(Span *span)
{
    s32 diff = span->end - span->start;

    if (diff < 0) {
        return diff - 1;
    }
    return diff + 1;
}

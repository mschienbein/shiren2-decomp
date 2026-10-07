#include "common.h"

typedef struct { s32 start; s32 end; } Range;
typedef struct { Range *range; signed char active; } S;
s32 func_80094B98(S *p) {
    s32 inactive = p->active != 1;
    if (inactive) return 0;
    return p->range->end - p->range->start > 0;
}

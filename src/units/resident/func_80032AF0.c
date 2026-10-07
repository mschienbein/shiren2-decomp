#include "common.h"

/* libultra osSpTaskYielded */

typedef struct {
    u32 type;
    u32 flags;
} OSTask_t;

typedef union {
    OSTask_t t;
    long long force_structure_alignment;
} OSTask;

u32 func_80032720(void);

u32 func_80032AF0(OSTask *tp)
{
    u32 status;
    u32 result;

    status = func_80032720();
    result = (status & 0x100) ? 1 : 0;
    if (status & 0x80) {
        tp->t.flags |= result;
        tp->t.flags &= ~2;
    }
    return result;
}

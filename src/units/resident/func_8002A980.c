#include "common.h"

typedef signed short s16;

/* Per-TU command view; the filter stores callable pointers, not integers. */
typedef union {
    struct { u32 w0; u32 w1; } words;
    long long force_structure_alignment;
} Acmd;

typedef Acmd *(*ALCmdHandler)(void *, s16 *, s32, s32, Acmd *);
typedef s32 (*ALSetParam)(void *, s32, void *);

typedef struct ALFilter_s {
    struct ALFilter_s *source;
    ALCmdHandler handler;
    ALSetParam setParam;
    s16 inp;
    s16 outp;
    s32 type;
} ALFilter;

void func_8002A980(void *filter, ALCmdHandler pull, ALSetParam param, s32 type)
{
    ALFilter *f = (ALFilter *)filter;

    f->source = 0;
    f->handler = pull;
    f->setParam = param;
    f->inp = 0;
    f->outp = 0;
    f->type = type;
}

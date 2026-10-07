#include "common.h"

typedef struct {
    u32 w0;
    u32 w1;
} Acmd;

typedef struct ALFilter {
    struct ALFilter *source;
    Acmd *(*handler)(void *filter, short *outp, s32 outCount, s32 sampleOffset, Acmd *p);
    s32 (*setParam)(void *filter, s32 paramID, void *param);
    short inp;
    short outp;
    s32 type;
} ALFilter;

typedef struct {
    ALFilter filter;
    short *dramout;
} ALSave;

Acmd *func_80031150(void *filter, short *outp, s32 outCount, s32 sampleOffset, Acmd *p)
{
    Acmd *ptr = p;
    ALSave *f = (ALSave *)filter;
    ALFilter *source = f->filter.source;

    ptr = (*source->handler)(source, outp, outCount, sampleOffset, ptr);

    {
        Acmd *_a = ptr++;
        _a->w0 = 0x08000000;
        _a->w1 = ((u32)(outCount << 1) & 0xFFFF);
    }
    {
        Acmd *_a = ptr++;
        _a->w0 = 0x0D000000;
        _a->w1 = 0x04400580;
    }
    {
        Acmd *_a = ptr++;
        _a->w0 = 0x08000000;
        _a->w1 = ((u32)(outCount << 2) & 0xFFFF);
    }
    {
        Acmd *_a = ptr++;
        _a->w0 = 0x06000000;
        _a->w1 = (u32)f->dramout;
    }
    return ptr;
}

s32 func_800311F8(void *filter, s32 paramID, void *param)
{
    ALSave *a = (ALSave *)filter;
    ALFilter *f = (ALFilter *)filter;

    switch (paramID) {
    case 1:
        f->source = (ALFilter *)param;
        break;
    case 6:
        a->dramout = param;
        break;
    }
    return 0;
}

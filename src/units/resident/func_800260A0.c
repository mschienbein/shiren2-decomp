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
    s32 sourceCount;
    s32 maxSources;
    ALFilter **sources;
} ALMainBus;

Acmd *func_800260A0(void *filter, short *outp, s32 outCount, s32 sampleOffset, Acmd *p)
{
    Acmd *ptr = p;
    ALMainBus *m = (ALMainBus *)filter;
    ALFilter **sources = m->sources;
    Acmd *cmd;
    s32 i;

    cmd = ptr++;
    cmd->w0 = 0x020006C0;
    cmd->w1 = outCount << 1;
    cmd = ptr++;
    cmd->w0 = 0x02000800;
    cmd->w1 = outCount << 1;

    for (i = 0; i < m->sourceCount; i++) {
        ptr = (*sources[i]->handler)(sources[i], outp, outCount, sampleOffset, ptr);
    }
    return ptr;
}

s32 func_8002617C(void *filter, s32 paramID, void *param)
{
    ALMainBus *m = (ALMainBus *)filter;
    ALFilter **sources = m->sources;

    switch (paramID) {
    case 2:
        sources[m->sourceCount++] = (ALFilter *)param;
        break;
    default:
        break;
    }
    return 0;
}

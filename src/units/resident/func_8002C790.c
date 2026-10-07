#include "common.h"

typedef struct ALFilter_s ALFilter;

/* Partial main-bus view: the 0x14-byte filter header is not used here. */
typedef struct {
    char filter[0x14];
    s32 sourceCount;
    s32 maxSources;
    ALFilter **sources;
} ALMainBus;

s32 func_8002C790(void *filter, s32 paramID, void *param)
{
    ALMainBus *m = (ALMainBus *)filter;
    ALFilter **sources = m->sources;

    if (paramID == 2) {
        sources[m->sourceCount++] = (ALFilter *)param;
    }
    return 0;
}

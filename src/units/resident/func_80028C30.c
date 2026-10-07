#include "common.h"

/* drvrNew.c tail: alAuxBusNew, alMainBusNew, alSaveNew */

typedef signed short s16;

/* Per-TU command view; callbacks use the same five-argument pull ABI. */
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

typedef struct {
    ALFilter filter;
    s32 sourceCount;
    s32 maxSources;
    ALFilter **sources;
} ALBus;

typedef struct {
    ALFilter filter;
    s16 *dramout;
    s32 first;
} ALSave;

extern void func_8002A980(void *f, ALCmdHandler pull, ALSetParam param, s32 type); /* alFilterNew */
extern Acmd *func_800260A0(void *filter, s16 *outp, s32 outCount, s32 sampleOffset, Acmd *p); /* alAuxBusPull */
extern s32 func_8002617C(void *filter, s32 paramID, void *param); /* alAuxBusParam */
extern Acmd *func_8002C650(void *filter, s16 *outp, s32 outCount, s32 sampleOffset, Acmd *p); /* alMainBusPull */
extern s32 func_8002C790(void *filter, s32 paramID, void *param); /* alMainBusParam */
extern Acmd *func_80031150(void *filter, s16 *outp, s32 outCount, s32 sampleOffset, Acmd *p); /* alSavePull */
extern s32 func_800311F8(void *filter, s32 paramID, void *param); /* alSaveParam */

void func_80028C30(ALBus *m, void *sources, s32 maxSources)
{
    func_8002A980(m, func_800260A0, func_8002617C, 6);
    m->sourceCount = 0;
    m->maxSources = maxSources;
    m->sources = (ALFilter **)sources;
}

void func_80028C8C(ALBus *m, void *sources, s32 maxSources)
{
    func_8002A980(m, func_8002C650, func_8002C790, 7);
    m->sourceCount = 0;
    m->maxSources = maxSources;
    m->sources = (ALFilter **)sources;
}

void func_80028CE8(ALSave *f)
{
    func_8002A980(f, func_80031150, func_800311F8, 3);
    f->dramout = 0;
    f->first = 1;
}

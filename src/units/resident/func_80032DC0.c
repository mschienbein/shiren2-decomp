#include "common.h"

typedef signed short s16;
typedef unsigned short u16;
typedef signed char s8;
typedef unsigned char u8;
typedef float f32;

/* alSynAllocFX (libultra libaudio synallocfx.c) */

typedef union {
    struct { u32 w0; u32 w1; } words;
    long long force_structure_alignment;
} Acmd;

typedef Acmd *(*ALCmdHandler)(void *, s16 *, s32, s32, Acmd *);
typedef s32 (*ALSetParam)(void *, s32, void *);
typedef s32 (*ALDMAproc)(u8 *addr, s32 len, void *state);
typedef ALDMAproc (*ALDMANew)(void **state);

typedef struct ALFilter_s {
    struct ALFilter_s *source;
    ALCmdHandler handler;
    ALSetParam setParam;
    s16 inp;
    s16 outp;
    s32 type;
} ALFilter;

typedef struct ALDelay_s ALDelay;
typedef struct {
    ALFilter filter;
    s16 *base;
    s16 *input;
    u32 length;
    ALDelay *delay;
    u8 section_count;
    ALSetParam paramHdl;
} ALFx;

typedef struct {
    ALFilter filter;
    s32 sourceCount;
    s32 maxSources;
    ALFilter **sources;
} ALBus;

/* Same 0x4C aux-bus allocation as func_80033080: effect begins at +0x20. */
typedef struct {
    ALBus bus;
    ALFx fx;
} ALAuxBus;

typedef struct ALLink_s {
    struct ALLink_s *next;
    struct ALLink_s *prev;
} ALLink;

struct ALPlayer_s;
struct ALParam_s;
/* Parameter-only synthesizer prefix, through the two bus pointers. */
typedef struct Instance_80037320 {
    struct ALPlayer_s *head;
    ALLink pFreeList;
    ALLink pAllocList;
    ALLink pLameList;
    s32 paramSamples;
    s32 curSamples;
    ALDMANew dma;
    void *heap;
    struct ALParam_s *paramList;
    ALBus *mainBus;
    ALAuxBus *auxBus;
} ALSynth;

typedef struct Config_80033080 {
    s32 maxVVoices;
    s32 maxPVoices;
    s32 maxUpdates;
    s32 maxFXbusses;
    ALDMANew dmaproc;
    void *heap;
    s32 outputRate;
    u8 fxType;
    s32 *params;
} ALSynConfig;

void func_800285A4(ALFx *fx, ALSynConfig *c, void *hp);
s32 func_80030644(void *filter, s32 paramID, void *param);
s32 func_8002C790(void *filter, s32 paramID, void *param);

void *func_80032DC0(ALSynth *s, s16 bus, ALSynConfig *c, void *hp) {
    func_800285A4(&s->auxBus[bus].fx, c, hp);
    func_80030644(&s->auxBus[bus].fx, 1, &s->auxBus[bus].bus);
    func_8002C790(s->mainBus, 2, &s->auxBus[bus].fx);
    return &s->auxBus[bus].fx;
}

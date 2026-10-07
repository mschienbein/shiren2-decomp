#include "common.h"

/* libultra libaudio synthesizer.c: alSynNew, alAudioFrame, __allocParam,
 * __freeParam, _collectPVoices, _freePVoice, _timeToSamplesNoRound,
 * _timeToSamples, __nextSampleTime */

typedef signed short s16;
typedef unsigned short u16;
typedef unsigned char u8;
typedef float f32;

typedef struct ALLink_s {
    struct ALLink_s *next;
    struct ALLink_s *prev;
} ALLink;

typedef struct {
    u32 w0;
    u32 w1;
} Awords;

typedef union {
    Awords words;
    long long force_union_align;
} Acmd;

typedef struct ALFilter_s ALFilter;
typedef Acmd *(*ALCmdHandler)(void *filter, s16 *tmp, s32 outCount, s32 sampleOffset, Acmd *p);
typedef s32 (*ALSetParam)(void *filter, s32 paramID, void *param);
typedef s32 (*ALDMAproc)(u8 *addr, s32 len, void *state);
typedef ALDMAproc (*ALDMANew)(void **state);
struct ALFilter_s {
    ALFilter *source;
    ALCmdHandler handler;
    ALSetParam setParam;
    s16 inp;
    s16 outp;
    s32 type;
};

typedef struct ALParam_s {
    struct ALParam_s *next;
    s32 delta;
    s16 type;
    s32 data;
    s32 moredata;
    s32 stuff;
    s32 misc;
} ALParam;

typedef struct ALPlayer_s ALPlayer;
typedef s32 (*ALVoiceHandler)(void *client);
struct ALPlayer_s {
    ALPlayer *next;
    void *clientData;
    ALVoiceHandler handler;
    s32 callTime;
    s32 samplesLeft;
};

/* Concrete drvrNew.c layouts: 0x48 decoder, 0x34 resampler, 0x4C mixer. */
typedef struct {
    ALFilter filter;
    void *state;
    void *lstate;
    u32 loopStart;
    u32 loopEnd;
    u32 loopCount;
    void *table;
    s32 bookSize;
    ALDMAproc dma;
    void *dmaState;
    s32 sample;
    s32 lastsam;
    s32 first;
    u8 *memin;
} ALLoadFilter;

typedef struct {
    ALFilter filter;
    void *state;
    f32 ratio;
    s32 upitch;
    f32 delta;
    s32 first;
    void *ctrlList;
    void *ctrlTail;
    s32 motion;
} ALResampler;

typedef struct {
    ALFilter filter;
    void *state;
    s16 pan;
    s16 volume;
    s16 cvolL;
    s16 cvolR;
    s16 dryamt;
    s16 wetamt;
    u16 lratl;
    s16 lratm;
    s16 ltgt;
    u16 rratl;
    s16 rratm;
    s16 rtgt;
    s32 delta;
    s32 segEnd;
    s32 first;
    void *ctrlList;
    void *ctrlTail;
    ALFilter **sources;
    s32 motion;
} ALEnvMixer;

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
    ALLink node;
    void *vvoice;
    ALFilter *channelKnob;
    ALLoadFilter decoder;
    ALResampler resampler;
    ALEnvMixer envmixer;
    s32 offset;
} PVoice;

typedef struct {
    ALFilter filter;
    s16 *dramout;
    s32 first;
} ALSave;

typedef struct {
    ALFilter filter;
    s32 sourceCount;
    s32 maxSources;
    ALFilter **sources;
} ALBus;

/* 0x20 bus header followed by the concrete 0x2C effect. */
typedef struct {
    ALBus bus;
    ALFx fx;
} ALAuxBus;

typedef ALBus ALMainBus;

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

typedef struct Instance_80037320 {
    ALPlayer *head;
    ALLink pFreeList;
    ALLink pAllocList;
    ALLink pLameList;
    s32 paramSamples;
    s32 curSamples;
    ALDMANew dma;
    void *heap;
    ALParam *paramList;
    ALMainBus *mainBus;
    ALAuxBus *auxBus;
    ALFilter *outputFilter;
    s32 numPVoices;
    s32 maxAuxBusses;
    s32 outputRate;
    s32 maxOutSamples;
} ALSynth;

extern ALSynth *D_80037320;

void *func_8002AB40(u8 *file, s32 line, void *hp, s32 num, s32 size);
void func_80028CE8(ALSave *save);
void func_80028C30(ALBus *bus, void *sources, s32 maxSources);
void func_80028C8C(ALBus *bus, void *sources, s32 maxSources);
void *func_80032DC0(ALSynth *s, s16 bus, ALSynConfig *c, void *hp);
s32 func_8002C790(void *filter, s32 paramID, void *param);
void func_800326AC(ALLink *ln, ALLink *to);
void func_800326CC(ALLink *ln);
void func_80028AF4(ALLoadFilter *f, ALDMANew dma, void *hp);
s32 func_8002BC10(void *filter, s32 paramID, void *param);
void func_80028BA4(ALResampler *f, void *hp);
s32 func_800301BC(void *filter, s32 paramID, void *param);
void func_80028A3C(ALEnvMixer *f, void *hp);
s32 func_80029230(void *filter, s32 paramID, void *param);
s32 func_8002617C(void *filter, s32 paramID, void *param);
s32 func_800311F8(void *filter, s32 paramID, void *param);

void func_80033540(ALSynth *drvr);
s32 func_800335D4(ALSynth *synth, s32 micros);
s32 func_80033668(ALSynth *drvr, ALPlayer **client);

void func_80033080(ALSynth *drvr, ALSynConfig *c) {
    s32 i;
    PVoice *pv;
    PVoice *pvoices;
    void *hp = c->heap;
    ALSave *save;
    ALParam *params;
    ALParam *paramPtr;
    void *sources;

    drvr->head = 0;
    drvr->numPVoices = c->maxPVoices;
    drvr->curSamples = 0;
    drvr->paramSamples = 0;
    drvr->outputRate = c->outputRate;
    drvr->maxOutSamples = 160;
    drvr->dma = c->dmaproc;

    save = func_8002AB40(0, 0, hp, 1, sizeof(ALSave));
    func_80028CE8(save);
    drvr->outputFilter = &save->filter;

    drvr->auxBus = func_8002AB40(0, 0, hp, 1, sizeof(ALAuxBus));
    drvr->maxAuxBusses = 1;
    sources = func_8002AB40(0, 0, hp, c->maxPVoices, 4);
    func_80028C30(&drvr->auxBus->bus, sources, c->maxPVoices);

    drvr->mainBus = func_8002AB40(0, 0, hp, 1, sizeof(ALMainBus));
    sources = func_8002AB40(0, 0, hp, c->maxPVoices, 4);
    func_80028C8C(drvr->mainBus, sources, c->maxPVoices);

    if (c->fxType != 0) {
        func_80032DC0(drvr, 0, c, hp);
    } else {
        func_8002C790(drvr->mainBus, 2, drvr->auxBus);
    }

    drvr->pFreeList.next = 0;
    drvr->pFreeList.prev = 0;
    drvr->pLameList.next = 0;
    drvr->pLameList.prev = 0;
    drvr->pAllocList.next = 0;
    drvr->pAllocList.prev = 0;

    pvoices = func_8002AB40(0, 0, hp, c->maxPVoices, sizeof(PVoice));
    for (i = 0; i < c->maxPVoices; i++) {
        pv = &pvoices[i];
        func_800326AC((ALLink *)pv, &drvr->pFreeList);
        pv->vvoice = 0;

        func_80028AF4(&pv->decoder, drvr->dma, hp);
        func_8002BC10(&pv->decoder, 1, 0);

        func_80028BA4(&pv->resampler, hp);
        func_800301BC(&pv->resampler, 1, &pv->decoder);

        func_80028A3C(&pv->envmixer, hp);
        func_80029230(&pv->envmixer, 1, &pv->resampler);

        func_8002617C(drvr->auxBus, 2, &pv->envmixer);

        pv->channelKnob = &pv->envmixer.filter;
    }

    func_800311F8(save, 1, drvr->mainBus);

    params = func_8002AB40(0, 0, hp, c->maxUpdates, sizeof(ALParam));
    drvr->paramList = 0;
    for (i = 0; i < c->maxUpdates; i++) {
        paramPtr = &params[i];
        paramPtr->next = drvr->paramList;
        drvr->paramList = paramPtr;
    }

    drvr->heap = hp;
}

Acmd *func_8003334C(Acmd *cmdList, s32 *cmdLen, s16 *outBuf, s32 outLen) {
    ALPlayer *client;
    ALFilter *output;
    ALSynth *drvr = D_80037320;
    s16 tmp = 0;
    Acmd *cmdlEnd = cmdList;
    Acmd *cmdPtr;
    s32 nOut;
    s16 *lOutBuf = outBuf;

    if (drvr->head == 0) {
        *cmdLen = 0;
        return cmdList;
    }

    for (drvr->paramSamples = func_80033668(drvr, &client);
         drvr->paramSamples - drvr->curSamples < outLen;
         drvr->paramSamples = func_80033668(drvr, &client)) {
        drvr->paramSamples &= ~0xf;
        client->samplesLeft += func_800335D4(drvr, (*client->handler)(client));
    }

    drvr->paramSamples &= ~0xf;

    while (outLen > 0) {
        nOut = (drvr->maxOutSamples < outLen) ? drvr->maxOutSamples : outLen;

        cmdPtr = cmdlEnd;
        {
            Acmd *_a = (Acmd *)cmdPtr++;
            _a->words.w0 = 0x07000000;
            _a->words.w1 = 0;
        }
        output = drvr->outputFilter;
        (*output->setParam)(output, 6, lOutBuf);
        cmdlEnd = (*output->handler)(output, &tmp, nOut, drvr->curSamples, cmdPtr);

        outLen -= nOut;
        lOutBuf += nOut << 1;
        drvr->curSamples += nOut;
    }
    *cmdLen = (s32)(cmdlEnd - cmdList);

    func_80033540(drvr);

    return cmdlEnd;
}

ALParam *func_800334FC(void) {
    ALParam *update = 0;
    ALSynth *drvr = D_80037320;

    if (drvr->paramList) {
        update = drvr->paramList;
        drvr->paramList = drvr->paramList->next;
        update->next = 0;
    }
    return update;
}

void func_80033528(ALParam *param) {
    ALSynth *drvr = D_80037320;

    param->next = drvr->paramList;
    drvr->paramList = param;
}

void func_80033540(ALSynth *drvr) {
    ALLink *dl;

    while ((dl = drvr->pLameList.next) != 0) {
        func_800326CC(dl);
        func_800326AC(dl, &drvr->pFreeList);
    }
}

void func_80033594(ALSynth *drvr, PVoice *pvoice) {
    func_800326CC((ALLink *)pvoice);
    func_800326AC((ALLink *)pvoice, &drvr->pLameList);
}

s32 func_800335D4(ALSynth *synth, s32 micros) {
    f32 tmp = ((f32)micros) * synth->outputRate / 1000000.0 + 0.5;

    return (s32)tmp;
}

s32 func_8003361C(ALSynth *synth, s32 micros) {
    f32 tmp = ((f32)micros) * synth->outputRate / 1000000.0 + 0.5;

    return (s32)tmp & ~0xf;
}

s32 func_80033668(ALSynth *drvr, ALPlayer **client) {
    s32 delta = 0x7fffffff;
    ALPlayer *cl;

    *client = 0;

    for (cl = drvr->head; cl != 0; cl = cl->next) {
        if ((cl->samplesLeft - drvr->curSamples) < delta) {
            *client = cl;
            delta = cl->samplesLeft - drvr->curSamples;
        }
    }

    return (*client)->samplesLeft;
}

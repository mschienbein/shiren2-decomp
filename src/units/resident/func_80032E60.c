#include "common.h"

/* alSynAllocVoice + _allocatePVoice (libultra libaudio synallocvoice.c) */

typedef signed short s16;
typedef unsigned short u16;
typedef unsigned char u8;
typedef float f32;

typedef struct ALLink_s {
    struct ALLink_s *next;
    struct ALLink_s *prev;
} ALLink;

typedef struct ALFilter_s ALFilter;
typedef s32 (*ALSetParam)(void *filter, s32 paramID, void *param);
struct ALFilter_s {
    ALFilter *source;
    void *handler;
    ALSetParam setParam;
    s16 inp;
    s16 outp;
    s32 type;
};

typedef struct ALParam_s {
    struct ALParam_s *next;
    s32 delta;
    s16 type;
    union {
        f32 f;
        s32 i;
    } data;
    union {
        f32 f;
        s32 i;
    } moredata;
} ALParam;

typedef struct PVoice_s PVoice;

typedef struct ALVoice_s {
    ALLink node;
    PVoice *pvoice;
    void *table;
    void *clientPrivate;
    s16 state;
    s16 priority;
    s16 fxBus;
    s16 unityPitch;
} ALVoice;

struct PVoice_s {
    ALLink node;
    ALVoice *vvoice;
    ALFilter *channelKnob;
    u8 filters[0xD8 - 0x10];
    s32 offset;
};

typedef struct {
    s16 priority;
    s16 fxBus;
    u8 unityPitch;
} ALVoiceConfig;

typedef struct {
    void *head;
    ALLink pFreeList;
    ALLink pAllocList;
    ALLink pLameList;
    s32 paramSamples;
} ALSynth;

ALParam *func_800334FC(void);
void func_800326CC(ALLink *ln);
void func_800326AC(ALLink *ln, ALLink *to);

s32 func_80032F88(ALSynth *drvr, PVoice **pvoice, s16 priority);

s32 func_80032E60(ALSynth *drvr, ALVoice *voice, ALVoiceConfig *vc) {
    PVoice *pvoice = 0;
    ALFilter *f;
    ALParam *update;
    s32 stolen;

    voice->priority = vc->priority;
    voice->unityPitch = vc->unityPitch;
    voice->table = 0;
    voice->fxBus = vc->fxBus;
    voice->state = 0;
    voice->pvoice = 0;

    stolen = func_80032F88(drvr, &pvoice, vc->priority);

    if (pvoice) {
        f = pvoice->channelKnob;

        if (stolen) {
            pvoice->offset = 512;
            pvoice->vvoice->pvoice = 0;

            update = func_800334FC();
            update->delta = drvr->paramSamples;
            update->type = 11;
            update->data.i = 0;
            update->moredata.i = pvoice->offset - 64;
            (*f->setParam)(f, 3, update);

            update = func_800334FC();
            if (update) {
                update->delta = drvr->paramSamples + pvoice->offset;
                update->type = 15;
                update->next = 0;
                (*f->setParam)(f, 3, update);
            }
        } else {
            pvoice->offset = 0;
        }

        pvoice->vvoice = voice;
        voice->pvoice = pvoice;
    }

    return (pvoice != 0);
}

s32 func_80032F88(ALSynth *drvr, PVoice **pvoice, s16 priority) {
    ALLink *dl;
    PVoice *pv;
    s32 stolen = 0;

    if ((dl = drvr->pLameList.next) != 0) {
        *pvoice = (PVoice *)dl;
        func_800326CC(dl);
        func_800326AC(dl, &drvr->pAllocList);
    } else if ((dl = drvr->pFreeList.next) != 0) {
        *pvoice = (PVoice *)dl;
        func_800326CC(dl);
        func_800326AC(dl, &drvr->pAllocList);
    } else {
        for (dl = drvr->pAllocList.next; dl != 0; dl = dl->next) {
            pv = (PVoice *)dl;
            if ((pv->vvoice->priority <= priority) && (pv->offset == 0)) {
                *pvoice = pv;
                priority = pv->vvoice->priority;
                stolen = 1;
            }
        }
    }
    return stolen;
}

#include "common.h"

/* libultra audio load.c: alLoadParam / _decodeChunk */

typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;

typedef union {
    struct {
        u32 w0;
        u32 w1;
    } words;
    long long force_structure_alignment;
} Acmd;

#define _SHIFTL(v, s, w) ((unsigned int)(((unsigned int)(v) & ((0x01 << (w)) - 1)) << (s)))

#define A_ADPCM 1
#define A_CLEARBUFF 2
#define A_LOADBUFF 4
#define A_SETBUFF 8
#define A_DMEMMOVE 10
#define A_LOADADPCM 11
#define A_SETLOOP 15
#define A_LOOP 0x2

#define aSetBuffer(pkt, f, i, o, c)                                                        \
    {                                                                                      \
        Acmd *_a = (Acmd *)pkt;                                                            \
        _a->words.w0 = (_SHIFTL(A_SETBUFF, 24, 8) | _SHIFTL(f, 16, 8) | _SHIFTL(i, 0, 16)); \
        _a->words.w1 = _SHIFTL(o, 16, 16) | _SHIFTL(c, 0, 16);                             \
    }

#define aLoadBuffer(pkt, s)                            \
    {                                                  \
        Acmd *_a = (Acmd *)pkt;                        \
        _a->words.w0 = _SHIFTL(A_LOADBUFF, 24, 8);     \
        _a->words.w1 = (unsigned int)(s);              \
    }

#define aSetLoop(pkt, a)                               \
    {                                                  \
        Acmd *_a = (Acmd *)pkt;                        \
        _a->words.w0 = _SHIFTL(A_SETLOOP, 24, 8);      \
        _a->words.w1 = (unsigned int)(a);              \
    }

#define aADPCMdec(pkt, f, s)                                           \
    {                                                                  \
        Acmd *_a = (Acmd *)pkt;                                        \
        _a->words.w0 = _SHIFTL(A_ADPCM, 24, 8) | _SHIFTL(f, 16, 8);    \
        _a->words.w1 = (unsigned int)(s);                              \
    }

#define aDMEMMove(pkt, i, o, c)                                            \
    {                                                                      \
        Acmd *_a = (Acmd *)pkt;                                            \
        _a->words.w0 = _SHIFTL(A_DMEMMOVE, 24, 8) | _SHIFTL(i, 0, 24);     \
        _a->words.w1 = _SHIFTL(o, 16, 16) | _SHIFTL(c, 0, 16);             \
    }

#define aClearBuffer(pkt, d, c)                                            \
    {                                                                      \
        Acmd *_a = (Acmd *)pkt;                                            \
        _a->words.w0 = _SHIFTL(A_CLEARBUFF, 24, 8) | _SHIFTL(d, 0, 24);    \
        _a->words.w1 = (unsigned int)(c);                                  \
    }

#define aLoadADPCM(pkt, c, d)                                              \
    {                                                                      \
        Acmd *_a = (Acmd *)pkt;                                            \
        _a->words.w0 = _SHIFTL(A_LOADADPCM, 24, 8) | _SHIFTL(c, 0, 24);    \
        _a->words.w1 = (unsigned int)d;                                    \
    }

#define K0_TO_PHYS(x) ((u32)(x) & 0x1FFFFFFF)

#define ADPCMVSIZE 8
#define ADPCMFSIZE 16
#define ADPCMFBYTES 9
#define LFSAMPLES 4

#define AL_ADPCM_WAVE 0
#define AL_RAW16_WAVE 1
#define AL_FILTER_RESET 4
#define AL_FILTER_SET_WAVETABLE 5

#define MIN(a, b) (((a) < (b)) ? (a) : (b))

typedef s16 ADPCM_STATE[16];

typedef struct {
    u32 start;
    u32 end;
    u32 count;
    ADPCM_STATE state;
} ALADPCMloop;

typedef struct {
    s32 order;
    s32 npredictors;
    s16 book[1];
} ALADPCMBook;

typedef struct {
    u32 start;
    u32 end;
    u32 count;
} ALRawLoop;

typedef struct {
    ALADPCMloop *loop;
    ALADPCMBook *book;
} ALADPCMWaveInfo;

typedef struct {
    ALRawLoop *loop;
} ALRAWWaveInfo;

typedef struct ALWaveTable_s {
    u8 *base;
    s32 len;
    u8 type;
    u8 flags;
    union {
        ALADPCMWaveInfo adpcmWave;
        ALRAWWaveInfo rawWave;
    } waveInfo;
} ALWaveTable;

typedef s32 (*ALDMAproc)(u8 *addr, s32 len, void *state);

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

typedef struct ALLoadFilter_s {
    ALFilter filter;
    ADPCM_STATE *state;
    ADPCM_STATE *lstate;
    ALRawLoop loop;
    struct ALWaveTable_s *table;
    s32 bookSize;
    ALDMAproc dma;
    void *dmaState;
    s32 sample;
    s32 lastsam;
    s32 first;
    u8 *memin;
} ALLoadFilter;

extern Acmd *func_8002B430(void *filter, s16 *outp, s32 outCount, s32 sampleOffset, Acmd *p);
extern Acmd *func_8002B874(void *filter, s16 *outp, s32 outCount, s32 sampleOffset, Acmd *p);
extern void func_80027C20(void *src, void *dest, s32 len);

s32 func_8002BC10(void *filter, s32 paramID, void *param)
{
    ALLoadFilter *a = (ALLoadFilter *)filter;
    ALFilter *f = (ALFilter *)filter;

    switch (paramID) {
        case AL_FILTER_SET_WAVETABLE:
            a->table = (ALWaveTable *)param;
            a->memin = a->table->base;
            a->sample = 0;
            switch (a->table->type) {
                case AL_ADPCM_WAVE:
                    f->handler = func_8002B430;
                    a->table->len = ADPCMFBYTES * ((s32)(a->table->len / ADPCMFBYTES));
                    a->bookSize = 2 * a->table->waveInfo.adpcmWave.book->order *
                                  a->table->waveInfo.adpcmWave.book->npredictors * ADPCMVSIZE;
                    if (a->table->waveInfo.adpcmWave.loop) {
                        a->loop.start = a->table->waveInfo.adpcmWave.loop->start;
                        a->loop.end = a->table->waveInfo.adpcmWave.loop->end;
                        a->loop.count = a->table->waveInfo.adpcmWave.loop->count;
                        func_80027C20(a->table->waveInfo.adpcmWave.loop->state, a->lstate, sizeof(ADPCM_STATE));
                    } else {
                        a->loop.start = a->loop.end = a->loop.count = 0;
                    }
                    break;

                case AL_RAW16_WAVE:
                    f->handler = func_8002B874;
                    if (a->table->waveInfo.rawWave.loop) {
                        a->loop.start = a->table->waveInfo.rawWave.loop->start;
                        a->loop.end = a->table->waveInfo.rawWave.loop->end;
                        a->loop.count = a->table->waveInfo.rawWave.loop->count;
                    } else {
                        a->loop.start = a->loop.end = a->loop.count = 0;
                    }
                    break;

                default:
                    break;
            }
            break;

        case AL_FILTER_RESET:
            a->lastsam = 0;
            a->first = 1;
            a->sample = 0;
            if (a->table) {
                a->memin = a->table->base;
                if (a->table->type == AL_ADPCM_WAVE) {
                    if (a->table->waveInfo.adpcmWave.loop) {
                        a->loop.count = a->table->waveInfo.adpcmWave.loop->count;
                    }
                } else if (a->table->type == AL_RAW16_WAVE) {
                    if (a->table->waveInfo.rawWave.loop) {
                        a->loop.count = a->table->waveInfo.rawWave.loop->count;
                    }
                }
            }
            break;

        default:
            break;
    }
}

Acmd *func_8002BDBC(Acmd *ptr, ALLoadFilter *f, s32 tsam, s32 nbytes, s16 outp, s16 inp, u32 flags)
{
    s32 dramAlign, dramLoc;

    if (nbytes > 0) {
        dramLoc = (f->dma)(f->memin, nbytes, f->dmaState);
        dramAlign = dramLoc & 0x7;
        nbytes += dramAlign;
        aSetBuffer(ptr++, 0, inp, 0, nbytes + 8 - (nbytes & 0x7));
        aLoadBuffer(ptr++, dramLoc - dramAlign);
    } else {
        dramAlign = 0;
    }

    if (flags & A_LOOP) {
        aSetLoop(ptr++, K0_TO_PHYS(f->lstate));
    }

    aSetBuffer(ptr++, 0, inp + dramAlign, outp, tsam << 1);
    aADPCMdec(ptr++, flags, K0_TO_PHYS(f->state));
    f->first = 0;

    return ptr;
}

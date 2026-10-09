#include "common.h"

typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;

typedef struct {
    u32 w0;
    u32 w1;
} Acmd;

typedef u32 (*Func8012DmaProc)(u32 address, s32 length, void *state);

typedef struct {
    s32 order;
    s32 npredictors;
    s16 book[1]; /* libultra ALADPCMBook: variable size, order * npredictors * 8 */
} ADPCMBook;

typedef struct {
    u32 start;
    u32 end;
    u32 count;
} ADPCMLoop;

/* ALWaveTable layout used by this mixer. */
typedef struct {
    s32 base;               /* 0x00: ROM address of the sample data */
    s32 len;                /* 0x04 */
    u8 type;                /* 0x08 */
    u8 flags;               /* 0x09 */
    ADPCMLoop *loop;        /* 0x0C */
    ADPCMBook *book;        /* 0x10 */
} WaveTable;

/* ADPCM load filter (libultra ALLoadFilter layout as used by this mixer). */
typedef struct {
    u8 pad00[0xC];
    void *state;            /* 0x0C: ADPCM decoder state buffer */
    void *lstate;           /* 0x10: loop state buffer */
    ADPCMLoop loop;         /* 0x14 */
    WaveTable *table;       /* 0x20 */
    s32 bookSize;           /* 0x24 */
    Func8012DmaProc dma;    /* 0x28 */
    void *dmaState;         /* 0x2C */
    s32 sample;             /* 0x30 */
    s32 lastsam;            /* 0x34 */
    s32 first;              /* 0x38 */
    s32 memin;              /* 0x3C: ROM read position */
} LoadFilter;

#define ADPCMFSIZE 16
#define ADPCMFBYTES 9
#define LFSAMPLES 4
#define A_LOOP 2
#define MIN(a, b) (((a) < (b)) ? (a) : (b))

Acmd *func_8012F01C(Acmd *ptr, LoadFilter *f, s32 tsam, s32 nbytes, s16 outp, s16 inp, u32 flags);

/* alAdpcmPull: decode outCount samples (following the loop when it is
 * reached) into the DMEM buffer at *outp. */
Acmd *func_8012EA60(void *filter, s16 *outp, s32 outCount, Acmd *p)
{
    Acmd *ptr = p;
    s16 inp;
    s32 tsam;
    s32 nframes;
    s32 nbytes;
    s32 overFlow;
    s32 startZero;
    s32 nOver;
    s32 nSam;
    s32 op;
    s32 nLeft;
    s32 bEnd;
    s32 decoded = 0;
    s32 looped = 0;
    LoadFilter *f = (LoadFilter *)filter;

    if (outCount == 0) {
        return ptr;
    }

    inp = 0;
    {
        Acmd *a = ptr++;
        a->w0 = 0x0B000000 | (f->bookSize & 0xFFFFFF);
        a->w1 = (u32)f->table->book->book & 0x1FFFFFFF; /* K0_TO_PHYS */
    }

    looped = (outCount + f->sample > f->loop.end) && (f->loop.count != 0);
    if (looped) {
        nSam = f->loop.end - f->sample;
    } else {
        nSam = outCount;
    }
    if (f->lastsam) {
        nLeft = ADPCMFSIZE - f->lastsam;
    } else {
        nLeft = 0;
    }
    tsam = nSam - nLeft;
    if (tsam < 0) {
        tsam = 0;
    }
    nframes = (tsam + ADPCMFSIZE - 1) >> LFSAMPLES;
    nbytes = nframes * ADPCMFBYTES;

    if (looped) {
        ptr = func_8012F01C(ptr, f, tsam, nbytes, *outp, inp, f->first);
        if (f->lastsam) {
            *outp += f->lastsam << 1;
        } else {
            *outp += ADPCMFSIZE << 1;
        }
        f->lastsam = f->loop.start & 0xF;
        f->memin = f->table->base + ADPCMFBYTES * ((f->loop.start >> LFSAMPLES) + 1);
        f->sample = f->loop.start;
        bEnd = *outp;
        while (outCount > nSam) {
            op = (bEnd + ((nframes + 1) << (LFSAMPLES + 1)) + 16) & ~0x1F;
            bEnd += nSam << 1;
            outCount -= nSam;
            if (f->loop.count != (u32)-1 && f->loop.count != 0) {
                f->loop.count--;
            }
            nSam = MIN(outCount, f->loop.end - f->loop.start);
            tsam = nSam - ADPCMFSIZE + f->lastsam;
            if (tsam < 0) {
                tsam = 0;
            }
            nframes = (tsam + ADPCMFSIZE - 1) >> LFSAMPLES;
            nbytes = nframes * ADPCMFBYTES;
            ptr = func_8012F01C(ptr, f, tsam, nbytes, op, inp, f->first | A_LOOP);
            {
                Acmd *a = ptr++;
                a->w0 = 0x0A000000 | ((op + (f->lastsam << 1)) & 0xFFFFFF);
                a->w1 = (bEnd << 16) | ((nSam << 1) & 0xFFFF);
            }
        }
        f->lastsam = (outCount + f->lastsam) & 0xF;
        f->sample += outCount;
        f->memin += ADPCMFBYTES * nframes;
        return ptr;
    }

    nSam = nframes << LFSAMPLES;
    overFlow = f->memin + nbytes - (f->table->base + f->table->len);
    if (overFlow < 0) {
        overFlow = 0;
    }
    nOver = (overFlow / ADPCMFBYTES) << LFSAMPLES;
    if (nOver > nSam + nLeft) {
        nOver = nSam + nLeft;
    }
    nbytes -= overFlow;
    if (nOver - (nOver & 0xF) < outCount) {
        decoded = 1;
        ptr = func_8012F01C(ptr, f, nSam - nOver, nbytes, *outp, inp, f->first);
        if (f->lastsam) {
            *outp += f->lastsam << 1;
        } else {
            *outp += ADPCMFSIZE << 1;
        }
        f->lastsam = (outCount + f->lastsam) & 0xF;
        f->sample += outCount;
        f->memin += ADPCMFBYTES * nframes;
    } else {
        f->lastsam = 0;
        f->memin += ADPCMFBYTES * nframes;
    }
    if (nOver) {
        f->lastsam = 0;
        if (decoded) {
            startZero = (nLeft + nSam - nOver) << 1;
        } else {
            startZero = 0;
        }
        {
            Acmd *a = ptr++;
            a->w0 = 0x02000000 | ((startZero + *outp) & 0xFFFFFF);
            a->w1 = nOver << 1;
        }
    }
    return ptr;
}

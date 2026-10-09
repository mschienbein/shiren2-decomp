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

/* _decodeChunk: DMA nbytes of ADPCM data into DMEM at inp and decode tsam
 * samples to outp. */
Acmd *func_8012F01C(Acmd *ptr, LoadFilter *f, s32 tsam, s32 nbytes, s16 outp, s16 inp, u32 flags)
{
    s32 dramAlign;
    u32 dramLoc;

    if (nbytes > 0) {
        dramLoc = f->dma(f->memin, nbytes, f->dmaState);
        dramAlign = dramLoc & 0x7;
        nbytes += dramAlign;
        {
            Acmd *a = ptr++;
            a->w0 = 0x04000000 | (((nbytes + 8 - (nbytes & 0x7)) & 0xFFF) << 12) | (inp & 0xFFF);
            a->w1 = dramLoc - dramAlign;
        }
    } else {
        dramAlign = 0;
    }
    if (flags & 2 /* A_LOOP */) {
        Acmd *a = ptr++;
        a->w0 = 0x0F000000;
        a->w1 = (u32)f->lstate & 0x1FFFFFFF; /* K0_TO_PHYS */
    }
    {
        Acmd *a = ptr++;
        a->w0 = 0x01000000 | ((u32)f->state & 0xFFFFFF);
        a->w1 = (flags << 28) | (((tsam << 1) & 0xFFF) << 16) | (dramAlign << 12) | (outp & 0xFFF);
    }
    f->first = 0;
    return ptr;
}

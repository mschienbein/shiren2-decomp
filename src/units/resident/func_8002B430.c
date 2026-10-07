#include "common.h"

/* libultra audio load.c: alAdpcmPull / alRaw16Pull */

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

extern Acmd *func_8002BDBC(Acmd *ptr, ALLoadFilter *f, s32 tsam, s32 nbytes, s16 outp, s16 inp, u32 flags);

Acmd *func_8002B430(void *filter, s16 *outp, s32 outCount, s32 sampleOffset, Acmd *p) 
{
    Acmd        *ptr = p;
    s16         inp;
    s32         tsam;
    s32         nframes;
    s32         nbytes;
    s32         overFlow;
    s32         startZero;
    s32         nOver;
    s32         nSam;
    s32         op;
    s32         nLeft;
    s32         bEnd;
    s32         decoded = 0;
    s32         looped = 0;
    
    ALLoadFilter *f = (ALLoadFilter *)filter;


    if (outCount == 0)
        return ptr;

    inp = 0;
    aLoadADPCM(ptr++, f->bookSize,
               K0_TO_PHYS(f->table->waveInfo.adpcmWave.book->book));

    looped = (outCount + f->sample > f->loop.end) && (f->loop.count != 0);
    if (looped)
        nSam = f->loop.end - f->sample;
    else
        nSam = outCount;
    
    if (f->lastsam)
        nLeft = ADPCMFSIZE - f->lastsam;
    else
        nLeft = 0;
    tsam = nSam - nLeft;
    if (tsam<0) tsam = 0;
    
    nframes = (tsam+ADPCMFSIZE-1)>>LFSAMPLES;
    nbytes =  nframes*ADPCMFBYTES;

    if (looped){

        ptr = func_8002BDBC(ptr, f, tsam, nbytes, *outp, inp, f->first);

        /*
         * Fix up output pointer, which will be used as the input pointer
         * by the following module.
         */
        if (f->lastsam)
            *outp += (f->lastsam<<1);
        else
            *outp += (ADPCMFSIZE<<1);

        /*
         * Now fix up state info to reflect the loop start point
         */
        f->lastsam = f->loop.start &0xf;
        f->memin = f->table->base + ADPCMFBYTES *
            ((s32) (f->loop.start>>LFSAMPLES) + 1);
        f->sample = f->loop.start;

        bEnd = *outp;
        while (outCount > nSam){
            
            outCount -= nSam;
            
            /*
             * Put next one after the end of the last lot - on the
             * frame boundary (32 byte) after the end.
             */
            op = (bEnd + ((nframes+1)<<(LFSAMPLES+1))) & ~0x1f;

            /*
             * The actual end of data
             */
            bEnd += (nSam<<1);
            
            /*
             * -1 is loop forever - the loop count is not exact now
             * for small loops!
             */
            if ((f->loop.count != -1) && (f->loop.count != 0))
                f->loop.count--;
            
            /*
             * What's left to compute.
             */
            nSam = MIN(outCount, f->loop.end - f->loop.start);
            tsam = nSam - ADPCMFSIZE + f->lastsam;  
            if (tsam<0) tsam = 0;
            nframes = (tsam+ADPCMFSIZE-1)>>LFSAMPLES;
            nbytes =  nframes*ADPCMFBYTES;
            ptr = func_8002BDBC(ptr, f, tsam, nbytes, op, inp, f->first | A_LOOP);
            /*
             * Merge the two sections in DMEM.
             */
            aDMEMMove(ptr++, op+(f->lastsam<<1), bEnd, nSam<<1);

        }
        
        f->lastsam = (outCount + f->lastsam) & 0xf;
        f->sample += outCount;
        f->memin += ADPCMFBYTES*nframes;    
        return ptr;
    }

    /*
     * The unlooped case, which is executed most of the time
     */

    nSam = nframes<<LFSAMPLES;
    
    /*
     * overFlow is the number of bytes past the end
     * of the bitstream I try to generate
     */
    /* local-arithmetic-qualification: this look-ahead can pass the bitstream
     * end. Integer arithmetic avoids forming that out-of-bounds pointer and
     * preserves the original add/add/sub order rather than reassociating a
     * pointer difference. Only the byte-overflow count is retained here. */
    overFlow = (s32)f->memin + nbytes - ((s32)f->table->base + f->table->len);
    if (overFlow < 0)
        overFlow = 0;
    nOver = (overFlow/ADPCMFBYTES)<<LFSAMPLES;
    if (nOver > nSam + nLeft)
        nOver = nSam + nLeft;
    
    nbytes -= overFlow;

    if ((nOver - (nOver & 0xf))< outCount){
        decoded = 1;
        ptr = func_8002BDBC(ptr, f, nSam - nOver, nbytes, *outp, inp, f->first);
    
        if (f->lastsam)
            *outp += (f->lastsam<<1);
        else
            *outp += (ADPCMFSIZE<<1);

        f->lastsam = (outCount + f->lastsam) & 0xf;
        f->sample += outCount;
        f->memin += ADPCMFBYTES*nframes;    
    } else {        
        f->lastsam = 0;
        f->memin += ADPCMFBYTES*nframes;    
    }

    /*
     * Put zeros in if necessary
     */
    if (nOver){
        f->lastsam = 0;
        if (decoded)
            startZero = (nLeft + nSam - nOver)<<1;
        else
            startZero = 0;
        aClearBuffer(ptr++, startZero + *outp, nOver<<1);
    }

    return ptr;
}

Acmd *func_8002B874(void *filter, s16 *outp, s32 outCount, s32 sampleOffset, Acmd *p) 
{
    Acmd        *ptr = p;
    s32         nbytes;
    s32         dramLoc;
    s32         dramAlign;
    s32         dmemAlign;
    s32         overFlow;
    s32         startZero;
    s32         nSam;
    s32         op;
    
    ALLoadFilter *f = (ALLoadFilter *)filter;
    ALFilter *a = (ALFilter *) filter;

    if (outCount == 0)
        return ptr;
    
    if ((outCount + f->sample > f->loop.end) && (f->loop.count != 0)){

        nSam = f->loop.end - f->sample;
        nbytes = nSam<<1;
        if (nSam > 0){
            dramLoc = (f->dma)(f->memin, nbytes, f->dmaState);
            
            /*
             * Make sure enough is loaded into DMEM to take care
             * of 8 byte alignment
             */
            dramAlign = dramLoc & 0x7;
            nbytes += dramAlign;
            aSetBuffer(ptr++, 0, *outp, 0, nbytes + 8 - (nbytes & 0x7));
            aLoadBuffer(ptr++, dramLoc - dramAlign);
        } else 
            dramAlign = 0; 
            
        /*
         * Fix up output pointer to allow for dram alignment
         */
        *outp += dramAlign;
        
        f->memin = f->table->base + (f->loop.start<<1);
        f->sample = f->loop.start;
        op = *outp;
        
        while (outCount > nSam){

            op += (nSam<<1);
            outCount -= nSam;
            /*
             * -1 is loop forever
             */
            if ((f->loop.count != -1) && (f->loop.count != 0))
                f->loop.count--;
            
            /*
             * What to compute.
             */
            nSam = MIN(outCount, f->loop.end - f->loop.start);
            nbytes = nSam<<1;
        
            /*
             * Do the next section, same as last.
             */
            dramLoc = (f->dma)(f->memin, nbytes, f->dmaState);
            
            /*
             * Make sure enough is loaded into DMEM to take care
             * of 8 byte alignment
             */
            dramAlign = dramLoc & 0x7;
            nbytes += dramAlign;
            if (op & 0x7)
                dmemAlign = 8 - (op & 0x7);
            else
                dmemAlign = 0;

            aSetBuffer(ptr++, 0, op + dmemAlign, 0, nbytes + 8 - (nbytes & 0x7));
            aLoadBuffer(ptr++, dramLoc - dramAlign);

            /*
             * Merge the two sections in DMEM.
             */
            if (dramAlign || dmemAlign)
                aDMEMMove(ptr++, op+dramAlign+dmemAlign, op, nSam<<1);
            
        }
        
        f->sample += outCount;
        f->memin += (outCount<<1);
        
        return ptr;
    }

    /*
     * The unlooped case, which is executed most of the time
     *
     * overFlow is the number of bytes past the end
     * of the bitstream I try to generate
     */

    nbytes = outCount<<1;
    /* local-arithmetic-qualification: as in the ADPCM path, compute the
     * signed byte-overflow probe without forming a past-end look-ahead
     * pointer, retaining the original add/add/sub instruction order. */
    overFlow = (s32)f->memin + nbytes - ((s32)f->table->base + f->table->len);
    if (overFlow < 0)
        overFlow = 0;
    if (overFlow > nbytes)
        overFlow = nbytes;
    
    if (overFlow < nbytes){
        if (outCount > 0){
            nbytes -= overFlow;
            dramLoc = (f->dma)(f->memin, nbytes, f->dmaState);
            
            /*
             * Make sure enough is loaded into DMEM to take care
             * of 8 byte alignment
             */
            dramAlign = dramLoc & 0x7;
            nbytes += dramAlign;
            aSetBuffer(ptr++, 0, *outp, 0, nbytes + 8 - (nbytes & 0x7));
            aLoadBuffer(ptr++, dramLoc - dramAlign);
        } else      
            dramAlign = 0; 
        *outp += dramAlign;

        f->sample += outCount;
        f->memin += outCount<<1;    
    } else {        
        f->memin += outCount<<1;    
    }

    /*
     * Put zeros in if necessary
     */
    if (overFlow){
        startZero = (outCount<<1) - overFlow;
        if (startZero < 0)
            startZero = 0;
        aClearBuffer(ptr++, startZero + *outp, overFlow);
    }
    return ptr;
}

#include "common.h"

/* Boot TU: idle thread + ROM segment loader (game code). */

typedef unsigned char u8;
typedef unsigned short u16;
typedef void *OSMesg;

typedef struct OSMesgQueue {
    void *mtqueue;
    void *fullqueue;
    s32 validCount;
    s32 first;
    s32 msgCount;
    OSMesg *msg;
} OSMesgQueue;

typedef struct {
    u16 type;
    u8 pri;
    u8 status;
    OSMesgQueue *retQueue;
} OSIoMesgHdr;

typedef struct {
    OSIoMesgHdr hdr;
    void *dramAddr;
    u32 devAddr;
    u32 size;
    void *piHandle;
} OSIoMesg;

extern void func_80032040(void *t, s32 pri); /* osSetThreadPri */
extern void *func_80026680(void); /* osCartRomInit */
extern void func_80027EA0(OSMesgQueue *mq, OSMesg *msg, long count); /* osCreateMesgQueue */
extern void func_8002B000(void *vaddr, s32 nbytes); /* osInvalDCache */
extern void func_8002B0B0(void *vaddr, s32 nbytes); /* osInvalICache */
extern s32 func_800299E0(void *handle, OSIoMesg *mb, s32 direction); /* osEPiStartDma */
extern long func_8002FEA0(OSMesgQueue *mq, OSMesg *msg, long flag); /* osRecvMesg */

void func_80025DB0(void *arg)
{
    /* The thread entry argument is supplied by osCreateThread but unused. */
    func_80032040(0, 0);
    for (;;) {
    }
}

void func_80025DCC(void *romStart, void *vram, s32 size)
{
    OSIoMesg mb;
    OSMesgQueue mq;
    OSMesg msgBuf[1];
    OSMesg msg;
    void *handle;

    handle = func_80026680();
    func_80027EA0(&mq, msgBuf, 1);
    mb.hdr.pri = 0;
    mb.hdr.retQueue = &mq;
    mb.dramAddr = vram;
    /* Encode the ROM-bus address only at the DMA message boundary. */
    mb.devAddr = (u32)romStart;
    mb.size = size;
    func_8002B000(vram, size);
    func_8002B0B0(vram, size);
    func_800299E0(handle, &mb, 0);
    func_8002FEA0(&mq, &msg, 1);
}

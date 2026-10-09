#include "common.h"

typedef unsigned long long u64;
typedef void *OSMesg;
typedef s32 OSPri;
struct ProbeThread;
typedef struct {
    struct ProbeThread *receive_waiters;
    struct ProbeThread *send_waiters;
    long valid_count;
    long first;
    long capacity;
    OSMesg *messages;
} ControllerQueueView;
typedef ControllerQueueView OSMesgQueue;
typedef unsigned char u8;
typedef struct OSThread {
    u8 opaque[0x1B0];
} OSThread;

typedef struct PiHandleView PiHandleView;

typedef struct {
    s32 active;
    OSThread *thread;
    OSMesgQueue *cmdQueue;
    OSMesgQueue *evtQueue;
    OSMesgQueue *acsQueue;
    s32 (*dma)(s32, u32, void *, u32);
    long (*edma)(PiHandleView *, long, unsigned long, void *, unsigned long);
} OSDevMgr;

void func_80027EA0(OSMesgQueue *mq, OSMesg *msg, long count); /* osCreateMesgQueue */
void func_8002F6B0(void); /* __osPiCreateAccessQueue */
void func_80031E90(s32 event, OSMesgQueue *mq, OSMesg msg); /* osSetEventMesg */
OSPri func_8002AA90(OSThread *thread); /* osGetThreadPri */
void func_80032040(OSThread *thread, OSPri pri); /* osSetThreadPri */
u32 func_8002AF70(void); /* __osDisableInt */
void func_8002AFE0(u32 mask); /* __osRestoreInt */
void func_80027ED0(OSThread *thread, s32 id, void (*entry)(void *), void *arg, void *sp, OSPri pri); /* osCreateThread */
void func_80032B50(OSThread *thread); /* osStartThread */
void func_80028080(void *arg); /* __osDevMgrMain */
s32 func_8002F950(s32 direction, u32 devAddr, void *dramAddr, u32 size); /* osPiRawStartDma */
long func_80029A80(PiHandleView *handle, long direction, unsigned long devAddr, void *dramAddr, unsigned long size); /* osEPiRawStartDma */

extern OSDevMgr D_80037280;
extern s32 D_80037270;
extern OSMesgQueue D_80041100;
static OSThread piThread; /* 8003D9E0 */
static u64 piThreadStack[0x1000 / sizeof(u64)]; /* 8003DB90 */
static OSMesgQueue piEventQueue; /* 8003EB90 */
static OSMesg piEventBuf; /* 8003EBA8 */

void func_8002F7C0(OSPri pri, OSMesgQueue *cmdQ, OSMesg *cmdBuf, s32 cmdMsgCnt)
{
    u32 savedMask;
    OSPri oldPri;
    OSPri myPri;

    if (!D_80037280.active) {
        func_80027EA0(cmdQ, cmdBuf, cmdMsgCnt);
        func_80027EA0(&piEventQueue, &piEventBuf, 1);
        if (!D_80037270) {
            func_8002F6B0();
        }
        func_80031E90(8, &piEventQueue, (OSMesg)0x22222222);
        oldPri = -1;
        myPri = func_8002AA90(0);
        if (myPri < pri) {
            oldPri = myPri;
            func_80032040(0, pri);
        }
        savedMask = func_8002AF70();
        D_80037280.active = 1;
        D_80037280.thread = &piThread;
        D_80037280.cmdQueue = cmdQ;
        D_80037280.evtQueue = &piEventQueue;
        D_80037280.acsQueue = &D_80041100;
        D_80037280.dma = func_8002F950;
        D_80037280.edma = func_80029A80;
        func_80027ED0(&piThread, 0, func_80028080, &D_80037280, &piThreadStack[0x1000 / sizeof(u64)], pri);
        func_80032B50(&piThread);
        func_8002AFE0(savedMask);
        if (oldPri != -1) {
            func_80032040(0, oldPri);
        }
    }
}

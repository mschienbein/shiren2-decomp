#include "common.h"
#include "controller_queue_view.h"

typedef unsigned short u16;
typedef unsigned char u8;
typedef float f32;
typedef volatile u32 vu32;
typedef void *OSMesg;
typedef struct OSMesgQueue OSMesgQueue;

typedef struct {
    u32 ctrl;
    u32 width;
    u32 burst;
    u32 vSync;
    u32 hSync;
    u32 leap;
    u32 hStart;
    u32 xScale;
    u32 vCurrent;
} OSViCommonRegs;

typedef struct {
    u32 origin;
    u32 yScale;
    u32 vStart;
    u32 vBurst;
    u32 vIntr;
} OSViFieldRegs;

typedef struct {
    u8 type;
    OSViCommonRegs comRegs;
    OSViFieldRegs fldRegs[2];
} OSViMode;

typedef struct {
    f32 factor;
    u16 offset;
    u32 scale;
} __OSViScale;

typedef struct {
    u16 state;
    u16 retraceCount;
    void *framep;
    OSViMode *modep;
    u32 control;
    OSMesgQueue *msgq;
    OSMesg msg;
    __OSViScale x;
    __OSViScale y;
} __OSViContext;

extern __OSViContext *D_800373C0; /* __osViCurr */
extern __OSViContext *D_800373C4; /* __osViNext */

u32 func_8002AF70(void);
void func_8002AFE0(u32);

typedef unsigned long long u64;
typedef s32 OSPri;
typedef struct OSThread OSThread;

struct OSMesgQueue {
    OSThread *mtqueue;
    OSThread *fullqueue;
    s32 validCount;
    s32 first;
    s32 msgCount;
    OSMesg *msg;
};

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

typedef struct {
    s32 active;
    OSThread *thread;
    OSMesgQueue *cmdQueue;
    OSMesgQueue *evtQueue;
    OSMesgQueue *acsQueue;
    s32 (*dma)(s32, u32, void *, u32);
    s32 (*edma)(void *, s32, u32, void *, u32);
} OSDevMgr;

extern OSDevMgr D_800373D0;       /* __osViDevMgr */
extern u32 D_800373EC;            /* __additional_scanline */
extern u16 D_8003EC00;            /* viMgrMain: retrace */
extern OSThread D_8003EC08;       /* viThread */
/* viThreadStack: the 0x1000-byte stack at 0x8003EDC0..0x8003FDC0 (bound by
   PROVIDE(D_8003EDC0) inside the splat bss block that starts with viThread). */
extern u64 D_8003EDC0[0x1000 / sizeof(u64)];
extern OSMesgQueue D_8003FDC0;    /* viEventQueue */
extern OSMesg D_8003FDD8[5];      /* viEventBuf */
extern OSIoMesg D_8003FDF0;       /* viRetraceMsg */
extern OSIoMesg D_8003FE08;       /* viCounterMsg */
extern u32 D_80039000;            /* __osBaseCounter */
extern u32 D_80039004;            /* __osViIntrCount */
extern u64 D_80039010;            /* __osCurrentTime */

void func_80033700(void);
void func_80027EA0(OSMesgQueue *, OSMesg *, ControllerQueueS32);
void func_80031E90(s32, OSMesgQueue *, OSMesg);
OSPri func_8002AA90(OSThread *);
void func_80032040(OSThread *, OSPri);
void func_80027ED0(OSThread *, s32, void (*)(void *), void *, void *, OSPri);
void func_80033BB0(void);
void func_80032B50(OSThread *);
__OSViContext *func_80033D20(void);
ControllerQueueS32 func_8002FEA0(OSMesgQueue *, OSMesg *, ControllerQueueS32);
ControllerQueueS32 func_80031D50(OSMesgQueue *, OSMesg, ControllerQueueS32);
u32 func_8002A9B0(void);
void func_80033754(void);
void func_80034410(void);

void func_80033F50(void *arg);

void func_80033DB0(OSPri pri) {
    u32 savedMask;
    OSPri oldPri;
    OSPri myPri;

    if (D_800373D0.active == 0) {
        func_80033700();
        D_800373EC = 0;
        func_80027EA0(&D_8003FDC0, D_8003FDD8, 5);
        D_8003FDF0.hdr.type = 13;
        D_8003FDF0.hdr.pri = 0;
        D_8003FDF0.hdr.retQueue = 0;
        D_8003FE08.hdr.type = 14;
        D_8003FE08.hdr.pri = 0;
        D_8003FE08.hdr.retQueue = 0;
        func_80031E90(7, &D_8003FDC0, &D_8003FDF0);
        func_80031E90(3, &D_8003FDC0, &D_8003FE08);
        oldPri = -1;
        myPri = func_8002AA90(0);
        if (myPri < pri) {
            oldPri = myPri;
            func_80032040(0, pri);
        }
        savedMask = func_8002AF70();
        D_800373D0.active = 1;
        D_800373D0.thread = &D_8003EC08;
        D_800373D0.cmdQueue = &D_8003FDC0;
        D_800373D0.evtQueue = &D_8003FDC0;
        D_800373D0.acsQueue = 0;
        D_800373D0.dma = 0;
        D_800373D0.edma = 0;
        func_80027ED0(&D_8003EC08, 0, func_80033F50, &D_800373D0, &D_8003EDC0[0x1000 / sizeof(u64)], pri);
        func_80033BB0();
        func_80032B50(&D_8003EC08);
        func_8002AFE0(savedMask);
        if (oldPri != -1) {
            func_80032040(0, oldPri);
        }
    }
}

void func_80033F50(void *arg) {
    __OSViContext *vc;
    OSDevMgr *dm;
    OSIoMesg *mb;
    s32 first;
    u32 count;

    mb = 0;
    first = 0;
    vc = func_80033D20();
    D_8003EC00 = vc->retraceCount;
    if (D_8003EC00 == 0) {
        D_8003EC00 = 1;
    }
    dm = (OSDevMgr *)arg;
    while (1) {
        func_8002FEA0(dm->evtQueue, (OSMesg *)&mb, 1);
        switch (mb->hdr.type) {
        case 13:
            func_80034410();
            D_8003EC00--;
            if (D_8003EC00 == 0) {
                vc = func_80033D20();
                if (vc->msgq != 0) {
                    func_80031D50(vc->msgq, vc->msg, 0);
                }
                D_8003EC00 = vc->retraceCount;
            }
            D_80039004++;
            if (first) {
                count = func_8002A9B0();
                D_80039010 = count;
                first = 0;
            }
            count = D_80039000;
            D_80039000 = func_8002A9B0();
            count = D_80039000 - count;
            D_80039010 = D_80039010 + count;
            break;
        case 14:
            func_80033754();
            break;
        }
    }
}

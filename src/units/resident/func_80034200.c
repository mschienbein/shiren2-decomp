#include "common.h"

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

void func_80034200(u32 func) {
    u32 saveMask = func_8002AF70();
    if (func & 1) {
        D_800373C4->control |= 8;
    }
    if (func & 2) {
        D_800373C4->control &= ~8;
    }
    if (func & 4) {
        D_800373C4->control |= 4;
    }
    if (func & 8) {
        D_800373C4->control &= ~4;
    }
    if (func & 0x10) {
        D_800373C4->control |= 0x10;
    }
    if (func & 0x20) {
        D_800373C4->control &= ~0x10;
    }
    if (func & 0x40) {
        D_800373C4->control |= 0x10000;
        D_800373C4->control &= ~0x300;
    }
    if (func & 0x80) {
        D_800373C4->control &= ~0x10000;
        D_800373C4->control |= D_800373C4->modep->comRegs.ctrl & 0x300;
    }
    D_800373C4->state |= 8;
    func_8002AFE0(saveMask);
}

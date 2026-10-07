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

extern u32 D_800373EC; /* __additional_scanline */

u32 func_800340F0(void *);

void func_80034410(void) {
    OSViMode *m;
    __OSViContext *vc;
    u32 origin;
    u32 vStartCurrent;
    u32 nomValue;
    u32 field;
    u32 hStart;

    vc = D_800373C4;
    field = *(vu32 *)0xA4400010 & 1;
    m = vc->modep;
    origin = func_800340F0(vc->framep) + m->fldRegs[field].origin;
    if (vc->state & 2) {
        vc->x.scale |= m->comRegs.xScale & ~0xFFF;
    } else {
        vc->x.scale = m->comRegs.xScale;
    }
    if (vc->state & 4) {
        nomValue = m->fldRegs[field].yScale & 0xFFF;
        vc->y.scale = vc->y.factor * nomValue;
        vc->y.scale |= m->fldRegs[field].yScale & ~0xFFF;
    } else {
        vc->y.scale = m->fldRegs[field].yScale;
    }
    vStartCurrent = m->fldRegs[field].vStart - (D_800373EC << 16) + D_800373EC;
    hStart = m->comRegs.hStart;
    if (vc->state & 0x20) {
        hStart = 0;
    }
    if (vc->state & 0x40) {
        vc->y.scale = 0;
        origin = func_800340F0(vc->framep);
    }
    if (vc->state & 0x80) {
        vc->y.scale = (vc->y.offset << 16) & 0x3FF0000;
        origin = func_800340F0(vc->framep);
    }
    *(vu32 *)0xA4400004 = origin;
    *(vu32 *)0xA4400008 = m->comRegs.width;
    *(vu32 *)0xA4400014 = m->comRegs.burst;
    *(vu32 *)0xA4400018 = m->comRegs.vSync;
    *(vu32 *)0xA440001C = m->comRegs.hSync;
    *(vu32 *)0xA4400020 = m->comRegs.leap;
    *(vu32 *)0xA4400024 = hStart;
    *(vu32 *)0xA4400028 = vStartCurrent;
    *(vu32 *)0xA440002C = m->fldRegs[field].vBurst;
    *(vu32 *)0xA440000C = m->fldRegs[field].vIntr;
    *(vu32 *)0xA4400030 = vc->x.scale;
    *(vu32 *)0xA4400034 = vc->y.scale;
    *(vu32 *)0xA4400000 = vc->control;
    D_800373C4 = D_800373C0;
    D_800373C0 = vc;
    *D_800373C4 = *D_800373C0;
}

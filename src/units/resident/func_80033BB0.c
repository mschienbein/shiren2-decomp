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

extern __OSViContext D_80037360[2];
extern u32 D_80000300; /* osTvType */
extern OSViMode D_800373F0; /* osViModeMpalLan1 */
extern OSViMode D_80037440; /* osViModeNtscLan1 */
extern OSViMode D_80037490; /* osViModePalLan1 */

void func_800265E0(void *, s32);
void func_80034410(void);

void func_80033BB0(void) {
    func_800265E0(D_80037360, sizeof(D_80037360));
    D_800373C0 = &D_80037360[0];
    D_800373C4 = &D_80037360[1];
    D_800373C4->retraceCount = 1;
    D_800373C0->retraceCount = 1;
    D_800373C4->framep = (void *)0x80000000;
    D_800373C0->framep = (void *)0x80000000;
    if (D_80000300 == 0) {
        D_800373C4->modep = &D_80037490;
    } else if (D_80000300 == 2) {
        D_800373C4->modep = &D_800373F0;
    } else {
        D_800373C4->modep = &D_80037440;
    }
    D_800373C4->state = 0x20;
    D_800373C4->control = D_800373C4->modep->comRegs.ctrl;
    while (*(vu32 *)0xA4400010 > 10) {
    }
    *(vu32 *)0xA4400000 = 0;
    func_80034410();
}

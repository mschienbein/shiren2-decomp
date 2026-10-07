#include "common.h"

typedef unsigned short u16;
typedef unsigned long long u64;
typedef s32 OSPri;
typedef void *OSMesg;
typedef u64 OSTime;
typedef struct OSMesgQueue OSMesgQueue;

typedef struct OSThread {
    struct OSThread *next;
    OSPri priority;
    struct OSThread **queue;
    struct OSThread *tlnext;
    u16 state;
    u16 flags;
} OSThread;

extern OSThread *D_80037338; /* __osRunQueue */
extern OSThread *D_80037340; /* __osRunningThread */

u32 func_8002AF70(void);
void func_8002AFE0(u32);

typedef struct OSTimer {
    struct OSTimer *next;
    struct OSTimer *prev;
    OSTime interval;
    OSTime value;
    OSMesgQueue *mq;
    OSMesg msg;
} OSTimer;

extern OSTimer *D_80037350; /* __osTimerList */
extern u32 D_80039020;      /* __osTimerCounter */

u32 func_8002A9B0(void);
OSTime func_80033910(OSTimer *);
void func_80033890(OSTime);

s32 func_80032110(OSTimer *t, OSTime value, OSTime interval, OSMesgQueue *mq, OSMesg msg) {
    OSTime tim;
    OSTimer *spC;
    u32 count;
    u32 elapsed;
    u32 saveMask;

    t->next = 0;
    t->prev = 0;
    t->value = value;
    t->interval = interval;
    if (value == 0) {
        t->value = interval;
    }
    t->mq = mq;
    t->msg = msg;
    saveMask = func_8002AF70();
    if (D_80037350->next != D_80037350) {
        spC = D_80037350->next;
        count = func_8002A9B0();
        elapsed = count - D_80039020;
        if (elapsed < spC->value) {
            spC->value -= elapsed;
        } else {
            spC->value = 1;
        }
    }
    tim = func_80033910(t);
    func_80033890(D_80037350->next->value);
    func_8002AFE0(saveMask);
    return 0;
}

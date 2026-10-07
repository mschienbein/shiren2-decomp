#include "common.h"
#include "controller_queue_view.h"

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
extern u32 D_80039000;      /* __osBaseCounter */
extern u32 D_80039004;      /* __osViIntrCount */
extern OSTime D_80039010;   /* __osCurrentTime */
extern u32 D_80039020;      /* __osTimerCounter */

void func_80031E80(u32);
u32 func_8002A9B0(void);
ControllerQueueS32 func_80031D50(OSMesgQueue *, OSMesg, ControllerQueueS32);
void func_80033890(OSTime);
OSTime func_80033910(OSTimer *);

void func_80033700(void) {
    D_80039010 = 0;
    D_80039000 = 0;
    D_80039004 = 0;
    D_80037350->prev = D_80037350;
    D_80037350->next = D_80037350->prev;
    D_80037350->value = 0;
    D_80037350->interval = D_80037350->value;
    D_80037350->mq = 0;
    D_80037350->msg = 0;
}

void func_80033754(void) {
    OSTimer *t;
    u32 count;
    u32 elapsed;

    if (D_80037350->next == D_80037350) {
        return;
    }
    while (1) {
        t = D_80037350->next;
        if (t == D_80037350) {
            func_80031E80(0);
            D_80039020 = 0;
            break;
        }
        count = func_8002A9B0();
        elapsed = count - D_80039020;
        D_80039020 = count;
        if (elapsed < t->value) {
            t->value -= elapsed;
            func_80033890(t->value);
            return;
        } else {
            t->prev->next = t->next;
            t->next->prev = t->prev;
            t->next = 0;
            t->prev = 0;
            if (t->mq != 0) {
                func_80031D50(t->mq, t->msg, 0);
            }
            if (t->interval != 0) {
                t->value = t->interval;
                func_80033910(t);
            }
        }
    }
}

void func_80033890(OSTime tim) {
    OSTime newTime;
    u32 savedMask;

    if (tim < 468) {
        tim = 468;
    }
    savedMask = func_8002AF70();
    D_80039020 = func_8002A9B0();
    newTime = D_80039020 + tim;
    func_80031E80(newTime);
    func_8002AFE0(savedMask);
}

OSTime func_80033910(OSTimer *t) {
    OSTimer *timep;
    OSTime tim;
    u32 savedMask;

    savedMask = func_8002AF70();
    for (timep = D_80037350->next, tim = t->value;
         timep != D_80037350 && tim > timep->value;
         tim -= timep->value, timep = timep->next) {
    }
    t->value = tim;
    if (timep != D_80037350) {
        timep->value -= tim;
    }
    t->next = timep;
    t->prev = timep->prev;
    timep->prev->next = t;
    timep->prev = t;
    func_8002AFE0(savedMask);
    return tim;
}

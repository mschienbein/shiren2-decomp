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

void func_8002A794(OSThread **, OSThread *);
OSThread *func_8002A7DC(OSThread **);
void func_8002A7F4(void);
void func_8002A68C(OSThread **);

void func_80032B50(OSThread *t) {
    register u32 saveMask = func_8002AF70();

    switch (t->state) {
    case 8:
        t->state = 2;
        func_8002A794(&D_80037338, t);
        break;
    case 1:
        if (t->queue == 0 || t->queue == &D_80037338) {
            t->state = 2;
            func_8002A794(&D_80037338, t);
        } else {
            t->state = 8;
            func_8002A794(t->queue, t);
            func_8002A794(&D_80037338, func_8002A7DC(t->queue));
        }
        break;
    }
    if (D_80037340 == 0) {
        func_8002A7F4();
    } else if (D_80037340->priority < D_80037338->priority) {
        D_80037340->state = 2;
        func_8002A68C(&D_80037338);
    }
    func_8002AFE0(saveMask);
}

#include "common.h"

/* libultra osDestroyThread */

typedef unsigned short u16;
typedef long long s64;
typedef unsigned long long u64;
typedef s32 OSPri;
typedef s32 OSId;

/* One saved general-purpose register: the 64-bit image that __osDispatchThread
 * reloads with `ld`. Big-endian, so an address-bearing register holds its
 * sign-extension word in `addr.high` and the address itself in `addr.low`.
 */
typedef union {
    u64 bits;
    struct {
        s32 high;
        void *low;
    } addr;
} __OSRegister;

typedef struct {
    __OSRegister at, v0, v1, a0, a1, a2, a3;
    __OSRegister t0, t1, t2, t3, t4, t5, t6, t7;
    __OSRegister s0, s1, s2, s3, s4, s5, s6, s7;
    __OSRegister t8, t9, gp, sp, s8, ra;
    u64 lo, hi;
    u32 sr;
    void (*pc)(void *); /* resume address; the entry function for a new thread */
    u32 cause;
    u32 badvaddr;
    u32 rcp;
    u32 fpcsr;
} __OSThreadContext;

typedef struct OSThread {
    struct OSThread *next;
    OSPri priority;
    struct OSThread **queue;
    struct OSThread *tlnext;
    u16 state;
    u16 flags;
    OSId id;
    int fp;
    __OSThreadContext context;
} OSThread;

extern OSThread *D_8003733C; /* __osActiveQueue */
extern OSThread *D_80037340; /* __osRunningThread */

extern u32 func_8002AF70(void);      /* __osDisableInt */
extern void func_8002AFE0(u32 mask); /* __osRestoreInt */
extern void func_800336C0(OSThread **queue, OSThread *t); /* __osDequeueThread */
extern void func_8002A7F4(void);     /* __osDispatchThread */

void func_80027FA0(OSThread *t)
{
    register u32 saveMask;
    register OSThread *pred;
    register OSThread *succ;

    saveMask = func_8002AF70();

    if (t == 0) {
        t = D_80037340;
    } else if (t->state != 1) {
        func_800336C0(t->queue, t);
    }

    if (D_8003733C == t) {
        D_8003733C = D_8003733C->tlnext;
    } else {
        pred = D_8003733C;
        while (pred->priority != -1) {
            succ = pred->tlnext;
            if (succ == t) {
                pred->tlnext = t->tlnext;
                break;
            }
            pred = succ;
        }
    }

    if (t == D_80037340) {
        func_8002A7F4();
    }

    func_8002AFE0(saveMask);
}

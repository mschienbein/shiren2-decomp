#include "common.h"

/* libultra osCreateThread */

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
extern void D_8002A970(void); /* __osCleanupThread: return address of the thread entry */

extern u32 func_8002AF70(void);     /* __osDisableInt */
extern void func_8002AFE0(u32 mask); /* __osRestoreInt */

void func_80027ED0(OSThread *t, OSId id, void (*entry)(void *), void *arg, void *sp, OSPri p)
{
    register u32 saveMask;
    u32 mask;

    t->id = id;
    t->priority = p;
    t->next = 0;
    t->queue = 0;
    t->context.pc = entry;
    /* local-arithmetic-qualification: the initial a0/sp/ra images are 64-bit GPR
     * values, i.e. the sign-extended 32-bit addresses (sp minus 16 with a 64-bit
     * borrow), which only the integer register-bits view can express; the
     * addresses stay readable through each register's `addr.low` pointer view. */
    t->context.a0.bits = (s64)(s32)arg;
    t->context.sp.bits = (s64)(s32)sp - 16;
    t->context.ra.bits = (s64)(s32)D_8002A970;

    mask = 0x003FFF01;
    t->context.sr = (mask & 0xFF01) | 2;
    t->context.rcp = (mask & 0x003F0000) >> 16;
    t->context.fpcsr = 0x01000800;
    t->fp = 0;
    t->state = 1;
    t->flags = 0;

    saveMask = func_8002AF70();
    t->tlnext = D_8003733C;
    D_8003733C = t;
    func_8002AFE0(saveMask);
}

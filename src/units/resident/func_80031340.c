#include "common.h"
#include "controller_queue_view.h"

/* libultra sched.c (SDK scheduler) */

typedef unsigned char u8;
typedef unsigned long long u64;
typedef void *OSMesg;

typedef struct {
    void *mtqueue;
    void *fullqueue;
    s32 validCount;
    s32 first;
    s32 msgCount;
    OSMesg *msg;
} OSMesgQueue;

typedef struct {
    u32 type;
    u32 flags;
    u64 *ucode_boot;
    u32 ucode_boot_size;
    u64 *ucode;
    u32 ucode_size;
    u64 *ucode_data;
    u32 ucode_data_size;
    u64 *dram_stack;
    u32 dram_stack_size;
    u64 *output_buff;
    u64 *output_buff_size;
    u64 *data_ptr;
    u32 data_size;
    u64 *yield_data_ptr;
    u32 yield_data_size;
} OSTask_t;

typedef union {
    OSTask_t t;
    long long force_structure_alignment;
} OSTask;

typedef struct {
    short type;
    char misc[30];
} OSScMsg;

typedef struct OSScTask_s {
    struct OSScTask_s *next;
    u32 state;
    u32 flags;
    void *framebuffer;
    OSTask list;
    OSMesgQueue *msgQ;
    OSMesg msg;
} OSScTask;

typedef struct OSScClient_s {
    struct OSScClient_s *next;
    OSMesgQueue *msgQ;
} OSScClient;

typedef struct {
    char opaque[0x1B0];
} OSThread;

typedef struct {
    OSScMsg retraceMsg;
    OSScMsg prenmiMsg;
    OSMesgQueue interruptQ;
    OSMesg intBuf[8];
    OSMesgQueue cmdQ;
    OSMesg cmdMsgBuf[8];
    OSThread thread;
    OSScClient *clientList;
    OSScTask *audioListHead;
    OSScTask *gfxListHead;
    OSScTask *audioListTail;
    OSScTask *gfxListTail;
    OSScTask *curRSPTask;
    OSScTask *curRDPTask;
    u32 frameCount;
    s32 doAudio;
} OSSched;

typedef struct {
    char opaque[0x50];
} OSViMode;

#define VIDEO_MSG 666
#define RSP_DONE_MSG 667
#define RDP_DONE_MSG 668
#define PRE_NMI_MSG 669

#define OS_SC_NEEDS_RDP 0x1
#define OS_SC_NEEDS_RSP 0x2
#define OS_SC_PARALLEL_TASK 0x10
#define OS_SC_LAST_TASK 0x20
#define OS_SC_SWAPBUFFER 0x40
#define OS_SC_RCP_MASK 0x3
#define OS_SC_TYPE_MASK 0x7

#define OS_SC_DP 0x1
#define OS_SC_SP 0x2
#define OS_SC_YIELD 0x10
#define OS_SC_YIELDED 0x20

#define M_GFXTASK 1
#define M_AUDTASK 2

extern OSViMode D_800374E0[];
extern s32 D_800372F4;
extern s32 D_800372F8;
extern s32 D_800372FC;

void func_80027EA0(OSMesgQueue *mq, OSMesg *msg, ControllerQueueS32 count);
void func_80027ED0(OSThread *t, s32 id, void (*entry)(void *), void *arg, void *sp, s32 pri);
ControllerQueueS32 func_8002FEA0(OSMesgQueue *mq, OSMesg *msg, ControllerQueueS32 flag);
ControllerQueueS32 func_80031D50(OSMesgQueue *mq, OSMesg msg, ControllerQueueS32 flag);
void func_80031E90(s32 event, OSMesgQueue *mq, OSMesg msg);
u32 func_80031F90(u32 mask);
void func_80032B50(OSThread *t);
void func_80033DB0(s32 pri);
void func_800341B0(OSViMode *mode);
void func_80033CC0(u8 active);
void func_80034150(OSMesgQueue *mq, OSMesg msg, u32 retraceCount);
void *func_80033D30(void);
void *func_80033D70(void);
void func_800343C0(void *fb);
void func_800347A0(void);
void func_80032890(OSTask *t);
void func_80032A9C(OSTask *t);
void func_80032AD0(void);
u32 func_80032AF0(OSTask *t);
s32 func_80028460(void *buf, u64 size);

void func_80031578(void *arg);
void func_8003167C(OSSched *sc);
void func_8003177C(OSSched *sc);
void func_8003186C(OSSched *sc);
OSScTask *func_800318FC(OSScTask *t);
s32 func_80031950(OSSched *sc, OSScTask *t);
void func_800319DC(OSSched *sc, OSScTask *t);
void func_80031A34(OSSched *sc, OSScTask *sp, OSScTask *dp);
void func_80031B04(OSSched *sc);
s32 func_80031B3C(OSSched *sc, OSScTask **sp, OSScTask **dp, s32 availRCP);

void func_80031340(OSSched *sc, void *stack, s32 priority, u8 mode, u8 numFields)
{
    sc->curRSPTask = 0;
    sc->curRDPTask = 0;
    sc->clientList = 0;
    sc->frameCount = 0;
    sc->audioListHead = 0;
    sc->gfxListHead = 0;
    sc->audioListTail = 0;
    sc->gfxListTail = 0;
    sc->retraceMsg.type = 1;
    sc->prenmiMsg.type = 4;

    func_80027EA0(&sc->interruptQ, sc->intBuf, 8);
    func_80027EA0(&sc->cmdQ, sc->cmdMsgBuf, 8);

    func_80033DB0(0xFE);
    func_800341B0(&D_800374E0[mode]);
    func_80033CC0(1);

    func_80031E90(4, &sc->interruptQ, (OSMesg)RSP_DONE_MSG);
    func_80031E90(9, &sc->interruptQ, (OSMesg)RDP_DONE_MSG);
    func_80031E90(14, &sc->interruptQ, (OSMesg)PRE_NMI_MSG);
    func_80034150(&sc->interruptQ, (OSMesg)VIDEO_MSG, numFields);

    func_80027ED0(&sc->thread, 4, func_80031578, sc, stack, priority);
    func_80032B50(&sc->thread);
}

void func_80031488(OSSched *sc, OSScClient *c, OSMesgQueue *msgQ)
{
    u32 mask;

    mask = func_80031F90(1);
    c->msgQ = msgQ;
    c->next = sc->clientList;
    sc->clientList = c;
    func_80031F90(mask);
}

void func_800314E0(OSSched *sc, OSScClient *c)
{
    OSScClient *client = sc->clientList;
    OSScClient *prev = 0;
    u32 mask;

    mask = func_80031F90(1);
    while (client != 0) {
        if (client == c) {
            if (prev) {
                prev->next = c->next;
            } else {
                sc->clientList = c->next;
            }
            break;
        }
        prev = client;
        client = client->next;
    }
    func_80031F90(mask);
}

OSMesgQueue *func_80031570(OSSched *sc)
{
    return &sc->cmdQ;
}

void func_80031578(void *arg)
{
    OSMesg msg;
    OSSched *sc = (OSSched *)arg;
    OSScClient *client;

    while (1) {
        func_8002FEA0(&sc->interruptQ, &msg, 1);
        switch ((int)msg) {
            case VIDEO_MSG:
                func_8003167C(sc);
                break;
            case RSP_DONE_MSG:
                func_8003177C(sc);
                break;
            case RDP_DONE_MSG:
                func_8003186C(sc);
                break;
            case PRE_NMI_MSG:
                for (client = sc->clientList; client != 0; client = client->next) {
                    func_80031D50(client->msgQ, (OSMesg)&sc->prenmiMsg, 0);
                }
                break;
        }
    }
}

void func_8003167C(OSSched *sc)
{
    OSScTask *rspTask;
    OSScClient *client;
    s32 state;
    OSScTask *sp = 0;
    OSScTask *dp = 0;

    sc->frameCount++;

    while (func_8002FEA0(&sc->cmdQ, (OSMesg *)&rspTask, 0) != -1) {
        func_800319DC(sc, rspTask);
    }

    if (sc->doAudio && sc->curRSPTask) {
        func_80031B04(sc);
    } else {
        state = (sc->curRSPTask == 0) << 1;
        if (sc->curRDPTask == 0) {
            state |= 1;
        }
        if (func_80031B3C(sc, &sp, &dp, state) != state) {
            func_80031A34(sc, sp, dp);
        }
    }

    for (client = sc->clientList; client != 0; client = client->next) {
        func_80031D50(client->msgQ, (OSMesg)sc, 0);
    }
}

void func_8003177C(OSSched *sc)
{
    OSScTask *t;
    OSScTask *sp = 0;
    OSScTask *dp = 0;
    s32 state;

    t = sc->curRSPTask;
    sc->curRSPTask = 0;

    if ((t->state & OS_SC_YIELD) && func_80032AF0(&t->list)) {
        t->state |= OS_SC_YIELDED;
        if ((t->flags & OS_SC_TYPE_MASK) == 3) {
            t->next = sc->gfxListHead;
            sc->gfxListHead = t;
            if (sc->gfxListTail == 0) {
                sc->gfxListTail = t;
            }
        }
    } else {
        t->state &= ~OS_SC_NEEDS_RSP;
        func_80031950(sc, t);
    }

    state = (sc->curRSPTask == 0) << 1;
        if (sc->curRDPTask == 0) {
            state |= 1;
        }
    if (func_80031B3C(sc, &sp, &dp, state) != state) {
        func_80031A34(sc, sp, dp);
    }
}

void func_8003186C(OSSched *sc)
{
    OSScTask *t;
    OSScTask *sp = 0;
    OSScTask *dp = 0;
    s32 state;

    t = sc->curRDPTask;
    sc->curRDPTask = 0;

    t->state &= ~OS_SC_NEEDS_RDP;
    func_80031950(sc, t);

    state = (sc->curRSPTask == 0) << 1;
        if (sc->curRDPTask == 0) {
            state |= 1;
        }
    if (func_80031B3C(sc, &sp, &dp, state) != state) {
        func_80031A34(sc, sp, dp);
    }
}

__inline OSScTask *func_800318FC(OSScTask *t)
{
    if (t) {
        return (func_80033D30() == func_80033D70()) ? t : 0;
    }
    return 0;
}

s32 func_80031950(OSSched *sc, OSScTask *t)
{
    if ((t->state & OS_SC_RCP_MASK) == 0) {
        func_80031D50(t->msgQ, t->msg, 1);

        if (t->list.t.type == M_GFXTASK) {
            if ((t->flags & OS_SC_SWAPBUFFER) && (t->flags & OS_SC_LAST_TASK)) {
                if (D_800372FC) {
                    func_80033CC0(0);
                    D_800372FC = 0;
                }
                func_800343C0(t->framebuffer);
            }
        }
        return 1;
    }
    return 0;
}

void func_800319DC(OSSched *sc, OSScTask *t)
{
    long type = t->list.t.type;

    if (type == M_AUDTASK) {
        if (sc->audioListTail) {
            sc->audioListTail->next = t;
        } else {
            sc->audioListHead = t;
        }
        sc->audioListTail = t;
        sc->doAudio = 1;
    } else {
        if (sc->gfxListTail) {
            sc->gfxListTail->next = t;
        } else {
            sc->gfxListHead = t;
        }
        sc->gfxListTail = t;
    }

    t->next = 0;
    t->state = t->flags & OS_SC_RCP_MASK;
}

void func_80031A34(OSSched *sc, OSScTask *sp, OSScTask *dp)
{
    if (sp) {
        if (sp->list.t.type == M_AUDTASK) {
            func_800347A0();
        }
        sp->state &= ~(OS_SC_YIELD | OS_SC_YIELDED);
        func_80032890(&sp->list);
        func_80032A9C(&sp->list);
        sc->curRSPTask = sp;
        if (sp == dp) {
            sc->curRDPTask = dp;
        }
    }

    if (dp && (dp != sp)) {
        func_80028460(dp->list.t.output_buff, *dp->list.t.output_buff_size);
        D_800372F4 = 1;
        D_800372F8 = 0;
        sc->curRDPTask = dp;
    }
}

void func_80031B04(OSSched *sc)
{
    if (sc->curRSPTask->list.t.type == M_GFXTASK) {
        sc->curRSPTask->state |= OS_SC_YIELD;
        func_80032AD0();
    }
}

s32 func_80031B3C(OSSched *sc, OSScTask **sp, OSScTask **dp, s32 availRCP)
{
    s32 avail = availRCP;
    OSScTask *gfx = sc->gfxListHead;
    OSScTask *audio = sc->audioListHead;

    if (sc->doAudio && (avail & OS_SC_SP)) {
        if (gfx && (gfx->flags & OS_SC_PARALLEL_TASK)) {
            *sp = gfx;
            avail &= ~OS_SC_SP;
        } else {
            *sp = audio;
            avail &= ~OS_SC_SP;
            sc->doAudio = 0;
            sc->audioListHead = sc->audioListHead->next;
            if (sc->audioListHead == 0) {
                sc->audioListTail = 0;
            }
        }
    } else {
        if (func_800318FC(gfx)) {
            switch (gfx->flags & OS_SC_TYPE_MASK) {
                case 3:
                    if (gfx->state & OS_SC_YIELDED) {
                        if (avail & OS_SC_SP) {
                            *sp = gfx;
                            avail &= ~OS_SC_SP;
                            if (gfx->state & OS_SC_DP) {
                                *dp = gfx;
                                avail &= ~OS_SC_DP;
                            }
                            sc->gfxListHead = sc->gfxListHead->next;
                            if (sc->gfxListHead == 0) {
                                sc->gfxListTail = 0;
                            }
                        }
                    } else {
                        if (avail == (OS_SC_SP | OS_SC_DP)) {
                            *sp = *dp = gfx;
                            avail &= ~(OS_SC_SP | OS_SC_DP);
                            sc->gfxListHead = sc->gfxListHead->next;
                            if (sc->gfxListHead == 0) {
                                sc->gfxListTail = 0;
                            }
                        }
                    }
                    break;
                case 7:
                case 2:
                case 6:
                    if (gfx->state & OS_SC_SP) {
                        if (avail & OS_SC_SP) {
                            *sp = gfx;
                            avail &= ~OS_SC_SP;
                        }
                    } else if (gfx->state & OS_SC_DP) {
                        if (avail & OS_SC_DP) {
                            *dp = gfx;
                            avail &= ~OS_SC_DP;
                            sc->gfxListHead = sc->gfxListHead->next;
                            if (sc->gfxListHead == 0) {
                                sc->gfxListTail = 0;
                            }
                        }
                    }
                    break;
                case 1:
                    break;
            }
        }
    }

    if (avail != availRCP) {
        avail = func_80031B3C(sc, sp, dp, avail);
    }
    return avail;
}

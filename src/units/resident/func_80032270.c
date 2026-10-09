#include "common.h"

/* libultra siacs.c */

typedef void *OSMesg;

typedef struct {
    void *mtqueue;
    void *fullqueue;
    s32 validCount;
    s32 first;
    s32 msgCount;
    OSMesg *msg;
} OSMesgQueue;

extern u32 D_80037310;         /* __osSiAccessQueueEnabled */
extern OSMesg D_8003EBB0[];    /* siAccessBuf */
extern OSMesgQueue D_800413A8; /* __osSiAccessQueue */

void func_80027EA0(OSMesgQueue *mq, OSMesg *msg, long count);
long func_8002FEA0(OSMesgQueue *mq, OSMesg *msg, long flag);
long func_80031D50(OSMesgQueue *mq, OSMesg msg, long flag);

void func_80032270(void)
{
    D_80037310 = 1;
    func_80027EA0(&D_800413A8, D_8003EBB0, 1);
    func_80031D50(&D_800413A8, (OSMesg)0, 0);
}

void func_800322C4(void)
{
    OSMesg dummyMesg;

    if (!D_80037310) {
        func_80032270();
    }
    func_8002FEA0(&D_800413A8, &dummyMesg, 1);
}

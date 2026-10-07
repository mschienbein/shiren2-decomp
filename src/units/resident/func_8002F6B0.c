#include "common.h"
#include "controller_queue_view.h"

typedef void *OSMesg;
typedef struct OSMesgQueue OSMesgQueue;
void func_80027EA0(OSMesgQueue *mq, OSMesg *msg, ControllerQueueS32 count);
ControllerQueueS32 func_80031D50(OSMesgQueue *mq, OSMesg msg, ControllerQueueS32 flag);
ControllerQueueS32 func_8002FEA0(OSMesgQueue *mq, OSMesg *msg, ControllerQueueS32 flag);

extern s32 D_80037270;
extern OSMesgQueue D_80041100;
extern OSMesg D_8003D9D0[1];

void func_8002F6B0(void)
{
    D_80037270 = 1;
    func_80027EA0(&D_80041100, D_8003D9D0, 1);
    func_80031D50(&D_80041100, 0, 0);
}

void func_8002F704(void)
{
    OSMesg dummy;

    if (!D_80037270) {
        func_8002F6B0();
    }
    func_8002FEA0(&D_80041100, &dummy, 1);
}

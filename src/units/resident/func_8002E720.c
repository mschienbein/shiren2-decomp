#include "common.h"
#include "controller_queue_view.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef void *OSMesg;

typedef struct {
    u16 type;
    u8 status;
    u8 errno;
} OSContStatus;

extern u8 D_80036F64;
typedef struct ResidentPifRam {
    u32 ramarray[15];
    u32 pifstatus;
} ResidentPifRam;
extern ResidentPifRam D_80041350;

extern void func_8002E810(s32 channel, u8 cmd);
extern s32 func_80032500(s32 direction, void *dram);
extern ControllerQueueS32 func_8002FEA0(ControllerQueueView *queue, OSMesg *msg,
                                        ControllerQueueS32 flags);
extern void func_8002E8A4(s32 channel, OSContStatus *data);

s32 func_8002E720(void *queue, s32 channel)
{
    s32 ret = 0;
    OSMesg dummy;
    OSContStatus data;

    D_80036F64 = 250;
    func_8002E810(channel, 0);
    ret = func_80032500(1, &D_80041350);
    func_8002FEA0(queue, &dummy, 1);
    ret = func_80032500(0, &D_80041350);
    func_8002FEA0(queue, &dummy, 1);
    func_8002E8A4(channel, &data);
    if ((data.status & 1) && (data.status & 2)) {
        return 2;
    } else if (data.errno != 0 || (data.status & 1) == 0) {
        return 1;
    } else if (data.status & 4) {
        return 4;
    }
    return ret;
}

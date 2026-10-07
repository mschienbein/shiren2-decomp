#include "common.h"
#include "controller_queue_view.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef void *OSMesg;

typedef struct {
    u16 type;
    u8 pri;
    void *retQueue;
} OSIoMesgHdr;

typedef struct {
    OSIoMesgHdr hdr;
    void *dramAddr;
    u32 devAddr;
    u32 size;
    void *piHandle;
} OSIoMesg;

extern s32 D_80037280; /* __osPiDevMgr.active */
extern ControllerQueueView *func_8002F7A0(void); /* osPiGetCmdQueue */
extern ControllerQueueS32 func_8002B130(ControllerQueueView *mq, OSMesg msg, ControllerQueueS32 flag);
extern ControllerQueueS32 func_80031D50(ControllerQueueView *mq, OSMesg msg, ControllerQueueS32 flag);

/* osEPiStartDma */
s32 func_800299E0(void *pihandle, OSIoMesg *mb, s32 direction)
{
    register s32 ret;

    if (!D_80037280) {
        return -1;
    }
    mb->piHandle = pihandle;
    if (direction == 0) {
        mb->hdr.type = 15;
    } else {
        mb->hdr.type = 16;
    }
    if (mb->hdr.pri == 1) {
        ret = func_8002B130(func_8002F7A0(), (OSMesg)mb, 0);
    } else {
        ret = func_80031D50(func_8002F7A0(), (OSMesg)mb, 0);
    }
    return ret;
}

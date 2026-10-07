#include "common.h"
#include "controller_queue_view.h"

/* libultra contreaddata.c: osContStartReadData, osContGetReadData, __osPackReadData */

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;

typedef void *OSMesg;
typedef struct OSMesgQueue OSMesgQueue;

typedef struct {
    u32 ramarray[15];
    u32 pifstatus;
} OSPifRam;

typedef struct {
    u8 dummy;
    u8 txsize;
    u8 rxsize;
    u8 cmd;
    u16 button;
    s8 stick_x;
    s8 stick_y;
} __OSContReadFormat;

typedef struct {
    u16 button;
    s8 stick_x;
    s8 stick_y;
    u8 errno;
} OSContPad;

extern OSPifRam D_80040FD0; /* __osContPifRam */
extern u8 D_80039008;       /* __osMaxControllers */
extern u8 D_80039018;       /* __osContLastCmd */

extern void func_800322C4(void); /* __osSiGetAccess */
extern void func_80032330(void); /* __osSiRelAccess */
extern s32 func_80032500(s32 direction, void *dramAddr); /* __osSiRawStartDma */
extern ControllerQueueS32 func_8002FEA0(OSMesgQueue *mq, OSMesg *msg, ControllerQueueS32 flag); /* osRecvMesg */

void func_80027854(void);

s32 func_80027730(OSMesgQueue *mq)
{
    s32 ret = 0;

    func_800322C4();
    if (D_80039018 != 1) {
        func_80027854();
        ret = func_80032500(1, D_80040FD0.ramarray);
        func_8002FEA0(mq, 0, 1);
    }
    ret = func_80032500(0, D_80040FD0.ramarray);
    D_80039018 = 1;
    func_80032330();
    return ret;
}

void func_800277B8(OSContPad *data)
{
    u8 *ptr = (u8 *)D_80040FD0.ramarray;
    __OSContReadFormat readformat;
    int i;

    for (i = 0; i < D_80039008; i++, ptr += sizeof(__OSContReadFormat), data++) {
        readformat = *(__OSContReadFormat *)ptr;
        data->errno = (readformat.rxsize & 0xC0) >> 4;
        if (data->errno == 0) {
            data->button = readformat.button;
            data->stick_x = readformat.stick_x;
            data->stick_y = readformat.stick_y;
        }
    }
}

void func_80027854(void)
{
    u8 *ptr = (u8 *)D_80040FD0.ramarray;
    __OSContReadFormat readformat;
    int i;

    for (i = 0; i < 15; i++) {
        D_80040FD0.ramarray[i] = 0;
    }

    D_80040FD0.pifstatus = 1;
    readformat.dummy = 0xFF;
    readformat.txsize = 1;
    readformat.rxsize = 4;
    readformat.cmd = 1;
    readformat.button = 0xFFFF;
    readformat.stick_x = -1;
    readformat.stick_y = -1;
    for (i = 0; i < D_80039008; i++) {
        *(__OSContReadFormat *)ptr = readformat;
        ptr += sizeof(__OSContReadFormat);
    }
    *ptr = 0xFE;
}

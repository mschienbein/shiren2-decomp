#include "common.h"
#include "controller_queue_view.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef void *OSMesg;
typedef struct OSMesgQueue OSMesgQueue;

typedef struct ResidentPifRam {
    u32 ramarray[15];
    u32 pifstatus;
} ResidentPifRam;

typedef struct {
    u16 type;
    u8 status;
    u8 errno;
} OSContStatus;

typedef struct {
    u8 dummy;
    u8 txsize;
    u8 rxsize;
    u8 cmd;
    u8 typeh;
    u8 typel;
    u8 status;
    u8 dummy1;
} __OSContRequesFormat;

extern ResidentPifRam D_80041350; /* __osPfsPifRam */
extern u8 D_80039008; /* __osMaxControllers */
extern u8 D_80039018; /* __osContLastCmd */

void func_800322C4(void); /* __osSiGetAccess */
void func_80032330(void); /* __osSiRelAccess */
s32 func_80032500(s32 direction, void *dramAddr); /* __osSiRawStartDma */
ControllerQueueS32 func_8002FEA0(OSMesgQueue *mq, OSMesg *msg, ControllerQueueS32 flag); /* osRecvMesg */
void func_8002EE20(u8 *pattern, OSContStatus *data); /* __osPfsGetInitData */
void func_8002ED7C(u8 cmd);

/* osPfsIsPlug */
s32 func_8002EC00(OSMesgQueue *queue, u8 *pattern)
{
    s32 ret = 0;
    OSMesg dummy;
    u8 bitpattern;
    OSContStatus data[4];
    s32 channel;
    u8 bits = 0;
    s32 crc_error_cnt = 3;

    func_800322C4();
    do {
        func_8002ED7C(0);
        ret = func_80032500(1, &D_80041350);
        func_8002FEA0(queue, &dummy, 1);
        ret = func_80032500(0, &D_80041350);
        func_8002FEA0(queue, &dummy, 1);
        func_8002EE20(&bitpattern, data);
        for (channel = 0; channel < D_80039008; channel++) {
            if ((data[channel].status & 4) == 0) {
                crc_error_cnt--;
                break;
            }
        }
        if (channel == D_80039008) {
            crc_error_cnt = 0;
        }
    } while (crc_error_cnt > 0);
    for (channel = 0; channel < D_80039008; channel++) {
        if (data[channel].errno == 0 && (data[channel].status & 1) != 0) {
            bits |= 1 << channel;
        }
    }
    func_80032330();
    *pattern = bits;
    return ret;
}

/* __osPfsRequestData */
void func_8002ED7C(u8 cmd)
{
    u8 *ptr = (u8 *)&D_80041350;
    __OSContRequesFormat requestformat;
    s32 i;

    D_80039018 = cmd;
    D_80041350.pifstatus = 1;
    requestformat.dummy = 0xFF;
    requestformat.txsize = 1;
    requestformat.rxsize = 3;
    requestformat.cmd = cmd;
    requestformat.typeh = 0xFF;
    requestformat.typel = 0xFF;
    requestformat.status = 0xFF;
    requestformat.dummy1 = 0xFF;
    for (i = 0; i < D_80039008; i++) {
        *(__OSContRequesFormat *)ptr = requestformat;
        ptr += sizeof(requestformat);
    }
    *ptr = 0xFE;
}

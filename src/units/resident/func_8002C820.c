#include "common.h"
#include "controller_queue_view.h"

/* libultra motor.c: __osMotorAccess / osMotorInit */

typedef unsigned char u8;
typedef unsigned short u16;
typedef void *OSMesg;
typedef struct OSMesgQueue OSMesgQueue;

typedef struct {
    s32 status;
    OSMesgQueue *queue;
    s32 channel;
    u8 id[32];
    u8 label[32];
    s32 version;
    s32 dir_size;
    s32 inode_table;
    s32 minode_table;
    s32 dir_table;
    s32 inode_start_page;
    u8 banks;
    u8 activebank;
} OSPfs;

typedef struct {
    u32 ramarray[15];
    u32 pifstatus;
} OSPifRam;

typedef struct {
    u8 dummy;
    u8 txsize;
    u8 rxsize;
    u8 cmd;
    u8 addrh;
    u8 addrl;
    u8 data[32];
    u8 datacrc;
} __OSContRamReadFormat;

#define READFORMAT(ptr) ((__OSContRamReadFormat *)(ptr))

extern OSPifRam D_8003D8D0[4];
extern u8 D_80039018;

extern void func_800322C4(void);
extern void func_80032330(void);
extern s32 func_80032500(s32 direction, void *dramAddr);
extern ControllerQueueS32 func_8002FEA0(OSMesgQueue *mq, OSMesg *msg, ControllerQueueS32 flag);
extern s32 func_8002F640(OSPfs *pfs, u8 bank);
extern s32 func_80027330(OSMesgQueue *mq, s32 channel, u16 address, u8 *buffer);
/* The CRC definition takes u32 and masks the low 16 address bits itself. */
extern u8 func_80027DB0(u32 address);

s32 func_8002C820(OSPfs *pfs, s32 flag)
{
    int i;
    s32 ret;
    u8 *ptr = (u8 *)&D_8003D8D0[pfs->channel];

    if (!(pfs->status & 8)) {
        return 5;
    }

    func_800322C4();
    D_8003D8D0[pfs->channel].pifstatus = 1;
    ptr += pfs->channel;

    for (i = 0; i < 32; i++) {
        READFORMAT(ptr)->data[i] = flag;
    }

    D_80039018 = 0xFE;
    func_80032500(1, &D_8003D8D0[pfs->channel]);
    func_8002FEA0(pfs->queue, 0, 1);
    func_80032500(0, &D_8003D8D0[pfs->channel]);
    func_8002FEA0(pfs->queue, 0, 1);

    ret = READFORMAT(ptr)->rxsize & 0xC0;
    if (!ret) {
        if (!flag) {
            if (READFORMAT(ptr)->datacrc != 0) {
                ret = 4;
            }
        } else {
            if (READFORMAT(ptr)->datacrc != 0xEB) {
                ret = 4;
            }
        }
    }

    func_80032330();

    return ret;
}

s32 func_8002C964(OSMesgQueue *mq, OSPfs *pfs, int channel)
{
    s32 ret;
    u8 temp[32];
    u8 *ptr;
    __OSContRamReadFormat ramreadformat;
    int i;

    pfs->queue = mq;
    pfs->channel = channel;
    pfs->activebank = 0xFF;
    pfs->status = 0;

    ret = func_8002F640(pfs, 0xFE);
    if (ret == 2) {
        ret = func_8002F640(pfs, 0x80);
    }
    if (ret != 0) {
        return ret;
    }

    ret = func_80027330(mq, channel, 0x400, temp);
    if (ret == 2) {
        ret = 4;
    }
    if (ret != 0) {
        return ret;
    }

    if (temp[31] == 0xFE) {
        return 11;
    }

    ret = func_8002F640(pfs, 0x80);
    if (ret == 2) {
        ret = 4;
    }
    if (ret != 0) {
        return ret;
    }

    ret = func_80027330(mq, channel, 0x400, temp);
    if (ret == 2) {
        ret = 4;
    }
    if (ret != 0) {
        return ret;
    }

    if (temp[31] != 0x80) {
        return 11;
    }

    if (!(pfs->status & 8)) {
        ptr = (u8 *)D_8003D8D0[channel].ramarray;
        ramreadformat.dummy = 0xFF;
        ramreadformat.txsize = 0x23;
        ramreadformat.rxsize = 1;
        ramreadformat.cmd = 3;
        ramreadformat.addrh = 0x600 >> 3;
        ramreadformat.addrl = (u8)(func_80027DB0(0x600) | (0x600 << 5));

        if (channel != 0) {
            for (i = 0; i < channel; i++) {
                *ptr++ = 0;
            }
        }

        *READFORMAT(ptr) = ramreadformat;
        ptr += sizeof(__OSContRamReadFormat);
        ptr[0] = 0xFE;
    }

    pfs->status = 8;

    return 0;
}

#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

/* Serial interface PIF RAM image: 15 command words followed by the status word. */
typedef struct {
    u32 ramarray[15];
    u32 pifstatus;
} PifRam;

/* One 6-byte joybus request/response frame (tx, rx, command, type lo/hi, status). */
typedef struct {
    u8 txsize;
    u8 rxsize;
    u8 command;
    u8 type_low;
    u8 type_high;
    u8 status;
} ProbeFrame;

typedef struct {
    u16 type;
    u8 status;
    u8 errnum;
} DeviceStatus;

extern u8 D_80039018;
extern PifRam D_80041350;

void func_8002E810(s32 channel, u8 command)
{
    u8 *ptr;
    ProbeFrame request;
    s32 i;

    D_80039018 = 0xFE;
    D_80041350.pifstatus = 1;
    ptr = (u8 *)&D_80041350.ramarray;
    request.txsize = 1;
    request.rxsize = 3;
    request.command = command;
    request.type_low = 0xFF;
    request.type_high = 0xFF;
    request.status = 0xFF;
    for (i = 0; i < channel; i++) {
        *ptr++ = 0;
    }
    *(ProbeFrame *)ptr = request;
    ptr += sizeof(ProbeFrame);
    *ptr = 0xFE;
}

void func_8002E8A4(s32 channel, DeviceStatus *data)
{
    u8 *ptr = (u8 *)&D_80041350.ramarray;
    ProbeFrame response;
    s32 i;

    for (i = 0; i < channel; i++) {
        ptr++;
    }
    response = *(ProbeFrame *)ptr;
    data->errnum = (response.rxsize & 0xC0) >> 4;
    if (data->errnum == 0) {
        data->type = (response.type_high << 8) | response.type_low;
        data->status = response.status;
    }
}

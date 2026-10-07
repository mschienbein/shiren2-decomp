#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

/* Serial interface PIF RAM image: 15 command words followed by the status word. */
typedef struct {
    u32 ramarray[15];
    u32 pifstatus;
} PifRam;

/* One 8-byte controller request/response frame. */
typedef struct {
    u8 dummy;
    u8 txsize;
    u8 rxsize;
    u8 command;
    u8 type_low;
    u8 type_high;
    u8 status;
    u8 dummy1;
} ControllerFrame;

typedef struct {
    u16 type;
    u8 status;
    u8 errnum;
} DeviceStatus;

extern u8 D_80039008;
extern PifRam D_80041350;

void func_8002EE20(u8 *pattern, DeviceStatus *data)
{
    u8 *ptr;
    ControllerFrame response;
    s32 i;
    u8 bits;

    bits = 0;
    ptr = (u8 *)&D_80041350.ramarray;

    for (i = 0; i < D_80039008; i++, ptr += sizeof(ControllerFrame), data++) {
        response = *(ControllerFrame *)ptr;
        data->errnum = (response.rxsize & 0xC0) >> 4;
        if (data->errnum == 0) {
            data->type = (response.type_high << 8) | response.type_low;
            data->status = response.status;
            bits |= 1 << i;
        }
    }
    *pattern = bits;
}

#include "common.h"

/* osCartRomInit */

typedef unsigned char u8;

typedef struct {
    u8 data[0x60];
} __OSTranxInfo;

typedef struct OSPiHandle {
    struct OSPiHandle *next;
    u8 type;
    u8 latency;
    u8 pageSize;
    u8 relDuration;
    u8 pulse;
    u8 domain;
    u32 baseAddress;
    u32 speed;
    __OSTranxInfo transferInfo;
} OSPiHandle;

extern s32 D_80036F50;          /* static first */
extern OSPiHandle D_80041088;   /* __CartRomHandle */
extern OSPiHandle *D_8003729C;  /* __osPiTable */

extern void func_8002F704(void); /* __osPiGetAccess */
extern void func_8002F770(void); /* __osPiRelAccess */
extern void func_800265E0(void *, s32); /* bzero */
extern u32 func_8002AF70(void); /* __osDisableInt */
extern void func_8002AFE0(u32); /* __osRestoreInt */

#define IO_READ(addr) (*(volatile u32 *)(addr))
#define IO_WRITE(addr, data) (*(volatile u32 *)(addr) = (u32)(data))

OSPiHandle *func_80026680(void)
{
    u32 value;
    u32 saveMask;
    register u32 stat;
    u32 latency;
    u32 pulse;
    u32 pageSize;
    u32 relDuration;

    func_8002F704();

    if (!D_80036F50) {
        func_8002F770();
        return &D_80041088;
    }

    D_80036F50 = 0;
    D_80041088.type = 0;
    D_80041088.baseAddress = 0xB0000000;
    D_80041088.domain = 0;
    D_80041088.speed = 0;

    func_800265E0(&D_80041088.transferInfo, sizeof(__OSTranxInfo));

    while (stat = IO_READ(0xA4600010), stat & 3) {
    }

    latency = IO_READ(0xA4600014);
    pageSize = IO_READ(0xA460001C);
    relDuration = IO_READ(0xA4600020);
    pulse = IO_READ(0xA4600018);

    IO_WRITE(0xA4600014, 0xFF);
    IO_WRITE(0xA460001C, 0);
    IO_WRITE(0xA4600020, 3);
    IO_WRITE(0xA4600018, 0xFF);

    value = IO_READ(D_80041088.baseAddress | 0xA0000000);
    D_80041088.latency = value & 0xFF;
    D_80041088.pageSize = (value >> 16) & 0xF;
    D_80041088.relDuration = (value >> 20) & 0xF;
    D_80041088.pulse = (value >> 8) & 0xFF;

    IO_WRITE(0xA4600014, latency);
    IO_WRITE(0xA460001C, pageSize);
    IO_WRITE(0xA4600020, relDuration);
    IO_WRITE(0xA4600018, pulse);

    saveMask = func_8002AF70();
    D_80041088.next = D_8003729C;
    D_8003729C = &D_80041088;
    func_8002AFE0(saveMask);
    func_8002F770();
    return &D_80041088;
}

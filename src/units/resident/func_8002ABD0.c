#include "common.h"

typedef unsigned char u8;
typedef unsigned long long u64;

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
} OSPiHandle;

typedef struct {
    u32 inst1;
    u32 inst2;
    u32 inst3;
    u32 inst4;
} __osExceptionVector;

#define IO_READ(addr) (*(volatile u32 *)((addr) | 0xA0000000))
#define IO_WRITE(addr, data) (*(volatile u32 *)((addr) | 0xA0000000) = (u32)(data))

#define PI_BSD_DOM1_LAT_REG 0x04600014
#define PI_BSD_DOM1_PWD_REG 0x04600018
#define PI_BSD_DOM1_PGS_REG 0x0460001C
#define PI_BSD_DOM1_RLS_REG 0x04600020
#define PI_BSD_DOM2_LAT_REG 0x04600024
#define PI_BSD_DOM2_PWD_REG 0x04600028
#define PI_BSD_DOM2_PGS_REG 0x0460002C
#define PI_BSD_DOM2_RLS_REG 0x04600030
#define AI_CONTROL_REG 0x04500008
#define AI_DACRATE_REG 0x04500010
#define AI_BITRATE_REG 0x04500014
#define PIF_RAM_END 0x1FC007FF

#define UT_VEC ((char *)0x80000000)
#define XUT_VEC ((char *)0x80000080)
#define ECC_VEC ((char *)0x80000100)
#define E_VEC ((char *)0x80000180)

extern OSPiHandle D_80041118; /* __Dom1SpeedParam */
extern OSPiHandle D_80041010; /* __Dom2SpeedParam */
extern s32 D_80039024;        /* __osFinalrom */
extern u64 D_80037250;        /* osClockRate */
extern s32 D_80037258;        /* osViClock */
extern s32 D_80000300;        /* osTvType */
extern s32 D_8000030C;        /* osResetType */
extern u8 D_8000031C[];       /* osAppNMIBuffer */

extern u32 func_8002AA60(void);            /* __osGetSR */
extern void func_80032030(u32 sr);         /* __osSetSR */
extern void func_80031F40(u32 csr);        /* __osSetFpcCsr */
extern void func_80032240(u32 value);      /* __osSetWatchLo */
extern s32 func_800325B0(u32 devAddr, u32 *data); /* __osSiRawReadIo */
extern s32 func_80032600(u32 devAddr, u32 data);  /* __osSiRawWriteIo */
extern void func_8002A010(void);           /* __osExceptionPreamble */
extern void func_80034720(void *addr, s32 size); /* osWritebackDCache */
extern void func_8002B0B0(void *addr, s32 size); /* osInvalICache */
extern void func_80033B60(void);           /* osUnmapTLBAll */
extern void func_8002C7C0(void);           /* osMapTLBRdb */
extern void func_800265E0(void *addr, s32 size); /* bzero */
extern u32 func_8002A9A0(void);            /* __osGetCause */

/* __createSpeedParam */
void func_8002ABD0(void)
{
    D_80041118.type = 7;
    D_80041118.latency = IO_READ(PI_BSD_DOM1_LAT_REG);
    D_80041118.pulse = IO_READ(PI_BSD_DOM1_PWD_REG);
    D_80041118.pageSize = IO_READ(PI_BSD_DOM1_PGS_REG);
    D_80041118.relDuration = IO_READ(PI_BSD_DOM1_RLS_REG);

    D_80041010.type = 7;
    D_80041010.latency = IO_READ(PI_BSD_DOM2_LAT_REG);
    D_80041010.pulse = IO_READ(PI_BSD_DOM2_PWD_REG);
    D_80041010.pageSize = IO_READ(PI_BSD_DOM2_PGS_REG);
    D_80041010.relDuration = IO_READ(PI_BSD_DOM2_RLS_REG);
}

/* osInitialize */
void func_8002AC88(void)
{
    u32 pifdata;

    D_80039024 = 1;
    func_80032030(func_8002AA60() | 0x20000000);
    func_80031F40(0x01000800);
    func_80032240(0x04900000);
    while (func_800325B0(PIF_RAM_END - 3, &pifdata)) {
    }
    while (func_80032600(PIF_RAM_END - 3, pifdata | 8)) {
    }
    *(__osExceptionVector *)UT_VEC = *(__osExceptionVector *)func_8002A010;
    *(__osExceptionVector *)XUT_VEC = *(__osExceptionVector *)func_8002A010;
    *(__osExceptionVector *)ECC_VEC = *(__osExceptionVector *)func_8002A010;
    *(__osExceptionVector *)E_VEC = *(__osExceptionVector *)func_8002A010;
    func_80034720(UT_VEC, E_VEC - UT_VEC + sizeof(__osExceptionVector));
    func_8002B0B0(UT_VEC, E_VEC - UT_VEC + sizeof(__osExceptionVector));
    func_8002ABD0();
    func_80033B60();
    func_8002C7C0();
    D_80037250 = D_80037250 * 3 / 4;
    if (D_8000030C == 0) {
        func_800265E0(D_8000031C, 64);
    }
    if (D_80000300 == 0) {
        D_80037258 = 49656530;
    } else if (D_80000300 == 2) {
        D_80037258 = 48628316;
    } else {
        D_80037258 = 48681812;
    }
    if (func_8002A9A0() & 0x1000) {
        while (1) {
        }
    }
    IO_WRITE(AI_CONTROL_REG, 1);
    IO_WRITE(AI_DACRATE_REG, 0x3FFF);
    IO_WRITE(AI_BITRATE_REG, 0xF);
}

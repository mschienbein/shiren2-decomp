#include "common.h"
#include "controller_queue_view.h"

/* libultra __osDevMgrMain (devmgr.c, 64DD-aware 2.0-era version) */

typedef unsigned char u8;
typedef unsigned short u16;
typedef void *OSMesg;
typedef struct OSMesgQueue_s OSMesgQueue;

/* Same long-based spellings as pi_handle_view.h's raw PI I/O definitions. */
typedef unsigned long PiWord;
typedef signed long PiResult;

typedef struct {
    u32 errStatus;
    void *dramAddr;
    void *C2Addr;
    u32 sectorSize;
    u32 C1ErrNum;
    u32 C1ErrSector[4];
} __OSBlockInfo;

typedef struct {
    u32 cmdType;
    u16 transferMode;
    u16 blockNum;
    s32 sectorNum;
    u32 devAddr;
    u32 bmCtlShadow;
    u32 seqCtlShadow;
    __OSBlockInfo block[2];
} __OSTranxInfo;

typedef struct OSPiHandle_s {
    struct OSPiHandle_s *next;
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

typedef struct {
    u16 type;
    u8 pri;
    u8 status;
    OSMesgQueue *retQueue;
} OSIoMesgHdr;

typedef struct {
    OSIoMesgHdr hdr;
    void *dramAddr;
    u32 devAddr;
    u32 size;
    OSPiHandle *piHandle;
} OSIoMesg;

typedef struct {
    s32 active;
    void *thread;
    OSMesgQueue *cmdQueue;
    OSMesgQueue *evtQueue;
    OSMesgQueue *acsQueue;
    /* +0x14: osCreatePiManager installs func_8002F950 (s32, u32, void *, u32 -> s32). */
    s32 (*dma)(s32, u32, void *, u32);
    /* +0x18: osCreatePiManager installs func_80029A80; same PiResult/PiWord
     * widths, with OSPiHandle as this TU's partial view of PiHandleView. */
    PiResult (*edma)(OSPiHandle *, PiResult, PiWord, void *, PiWord);
} OSDevMgr;

extern ControllerQueueS32 func_8002FEA0(OSMesgQueue *mq, OSMesg *msg, ControllerQueueS32 flag); /* osRecvMesg */
extern ControllerQueueS32 func_80031D50(OSMesgQueue *mq, OSMesg msg, ControllerQueueS32 flag);  /* osSendMesg */
extern void func_80030280(u32 mask);                               /* __osResetGlobalIntMask */
extern void func_80031F50(u32 mask);                               /* __osSetGlobalIntMask */
extern PiResult func_80029DE0(OSPiHandle *h, PiWord devAddr, PiWord data);   /* __osEPiRawWriteIo */
extern PiResult func_80029C70(OSPiHandle *h, PiWord devAddr, PiWord *data);  /* __osEPiRawReadIo */
extern void func_80035F00(void);                                   /* osYieldThread */

#define LEO_BM_CTL 0x05000510
#define LEO_STATUS 0x05000508
#define PI_STATUS_REG (*(volatile u32 *)0xA4600010)

void func_80028080(void *args)
{
    OSIoMesg *mb;
    OSMesg em;
    OSMesg dummy;
    s32 ret;
    OSDevMgr *dm;
    s32 messageSend = 0;

    dm = (OSDevMgr *)args;
    mb = 0;
    ret = 0;

    while (1) {
        func_8002FEA0(dm->cmdQueue, (OSMesg *)&mb, 1);

        if (mb->piHandle != 0 && mb->piHandle->type == 2 &&
            (mb->piHandle->transferInfo.cmdType == 0 || mb->piHandle->transferInfo.cmdType == 1)) {
            __OSBlockInfo *blockInfo;
            __OSTranxInfo *info;

            info = &mb->piHandle->transferInfo;
            blockInfo = &info->block[info->blockNum];
            info->sectorNum = -1;

            if (info->transferMode != 3) {
                blockInfo->dramAddr = (void *)((u32)blockInfo->dramAddr - blockInfo->sectorSize);
            }

            if (info->transferMode == 2 && mb->piHandle->transferInfo.cmdType == 0) {
                messageSend = 1;
            } else {
                messageSend = 0;
            }

            func_8002FEA0(dm->acsQueue, &dummy, 1);
            func_80030280(0x00100401);
            func_80029DE0(mb->piHandle, LEO_BM_CTL, info->bmCtlShadow | 0x80000000);

        readblock1:
            func_8002FEA0(dm->evtQueue, &em, 1);
            info = &mb->piHandle->transferInfo;
            blockInfo = &info->block[info->blockNum];

            if (blockInfo->errStatus == 29) {
                PiWord stat;

                func_80029DE0(mb->piHandle, LEO_BM_CTL, info->bmCtlShadow | 0x10000000);
                func_80029DE0(mb->piHandle, LEO_BM_CTL, info->bmCtlShadow);
                func_80029C70(mb->piHandle, LEO_STATUS, &stat);

                if (stat & 0x02000000) {
                    func_80029DE0(mb->piHandle, LEO_BM_CTL, info->bmCtlShadow | 0x01000000);
                }

                blockInfo->errStatus = 4;
                PI_STATUS_REG = 2;
                func_80031F50(0x00100C01);
            }

            func_80031D50(mb->hdr.retQueue, mb, 0);

            if (messageSend == 1 && mb->piHandle->transferInfo.block[0].errStatus == 0) {
                messageSend = 0;
                goto readblock1;
            }

            func_80031D50(dm->acsQueue, 0, 0);
            if (mb->piHandle->transferInfo.blockNum == 1) {
                func_80035F00();
            }
        } else {
            switch (mb->hdr.type) {
                case 11:
                    func_8002FEA0(dm->acsQueue, &dummy, 1);
                    ret = dm->dma(0, mb->devAddr, mb->dramAddr, mb->size);
                    break;
                case 12:
                    func_8002FEA0(dm->acsQueue, &dummy, 1);
                    ret = dm->dma(1, mb->devAddr, mb->dramAddr, mb->size);
                    break;
                case 15:
                    func_8002FEA0(dm->acsQueue, &dummy, 1);
                    ret = dm->edma(mb->piHandle, 0, mb->devAddr, mb->dramAddr, mb->size);
                    break;
                case 16:
                    func_8002FEA0(dm->acsQueue, &dummy, 1);
                    ret = dm->edma(mb->piHandle, 1, mb->devAddr, mb->dramAddr, mb->size);
                    break;
                case 10:
                    func_80031D50(mb->hdr.retQueue, mb, 0);
                    ret = -1;
                    break;
                default:
                    ret = -1;
                    break;
            }

            if (ret == 0) {
                func_8002FEA0(dm->evtQueue, &em, 1);
                func_80031D50(mb->hdr.retQueue, mb, 0);
                func_80031D50(dm->acsQueue, 0, 0);
            }
        }
    }
}

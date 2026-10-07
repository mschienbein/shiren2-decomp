#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef signed short s16;
typedef signed char s8;

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
    u32 game_code;
    u16 company_code;
    u16 start_page;
    u8 status;
    s8 reserved;
    u16 data_sum;
    u8 ext_name[4];
    u8 game_name[16];
} __OSDir;

typedef unsigned long long u64;

typedef struct {
    u32 repaired;
    u32 random;
    u64 serial_mid;
    u64 serial_low;
    u16 deviceid;
    u8 banks;
    u8 version;
    u16 checksum;
    u16 inverted_checksum;
} __OSPackId;

void func_800322C4(void); /* __osSiGetAccess */
void func_80032330(void); /* __osSiRelAccess */
s32 func_8002E720(OSMesgQueue *queue, s32 channel); /* __osPfsGetStatus */
s32 func_8002F640(OSPfs *pfs, u8 bank); /* __osPfsSelectBank */
s32 func_80027330(OSMesgQueue *queue, s32 channel, u16 address, u8 *buffer); /* __osContRamRead */
s32 func_80027520(OSMesgQueue *queue, s32 channel, u16 address, u8 *buffer, s32 force); /* __osContRamWrite */
s32 func_80026834(u16 *ptr, u16 *csum, u16 *icsum); /* __osIdCheckSum */
s32 func_80026B64(OSPfs *pfs, __OSPackId *check); /* __osCheckPackId */
s32 func_80026878(OSPfs *pfs, __OSPackId *badid, __OSPackId *newid); /* __osRepairPackId */
void *func_800262C0(const void *src, void *dst, s32 len); /* bcopy; returns dst */
s32 func_800261B0(const void *a, const void *b, s32 len); /* bcmp */
s32 func_8002DB30(OSPfs *pfs); /* osPfsChecker */

s32 func_8002EB28(OSPfs *pfs);

/* osPfsInitPak */
s32 func_8002E930(OSMesgQueue *queue, OSPfs *pfs, s32 channel)
{
    s32 ret;
    u16 sum;
    u16 isum;
    u8 temp[32];
    __OSPackId *id;
    __OSPackId newid;

    func_800322C4();
    ret = func_8002E720(queue, channel);
    func_80032330();
    if (ret != 0) {
        return ret;
    }
    pfs->queue = queue;
    pfs->channel = channel;
    pfs->status = 0;

    if ((ret = func_8002EB28(pfs)) != 0) {
        return ret;
    }
    if ((ret = func_8002F640(pfs, 0)) != 0) {
        return ret;
    }
    if ((ret = func_80027330(pfs->queue, pfs->channel, 1, temp)) != 0) {
        return ret;
    }
    func_80026834((u16 *)temp, &sum, &isum);
    id = (__OSPackId *)temp;
    if (id->checksum != sum || id->inverted_checksum != isum) {
        if ((ret = func_80026B64(pfs, id)) != 0) {
            pfs->status |= 4;
            return ret;
        }
    }
    if ((id->deviceid & 1) == 0) {
        if ((ret = func_80026878(pfs, id, &newid)) != 0) {
            if (ret == 10) {
                pfs->status |= 4;
            }
            return ret;
        }
        id = &newid;
        if ((id->deviceid & 1) == 0) {
            return 11;
        }
    }
    func_800262C0(id, pfs->id, 32);
    pfs->version = id->version;
    pfs->banks = id->banks;
    pfs->inode_start_page = 1 + 2 + 2 * pfs->banks;
    pfs->dir_size = 2 * 8;
    pfs->inode_table = 1 * 8;
    pfs->minode_table = (1 + pfs->banks) * 8;
    pfs->dir_table = pfs->minode_table + pfs->banks * 8;
    if ((ret = func_80027330(pfs->queue, pfs->channel, 7, pfs->label)) != 0) {
        return ret;
    }
    ret = func_8002DB30(pfs);
    pfs->status |= 1;
    return ret;
}

/* __osPfsCheckRamArea */
s32 func_8002EB28(OSPfs *pfs)
{
    s32 i = 0;
    s32 ret = 0;
    u8 temp1[32];
    u8 temp2[32];
    u8 save[32];

    if ((ret = func_8002F640(pfs, 0)) != 0) {
        return ret;
    }
    if ((ret = func_80027330(pfs->queue, pfs->channel, 0, save)) != 0) {
        return ret;
    }
    for (i = 0; i < 32; i++) {
        temp1[i] = i;
    }
    if ((ret = func_80027520(pfs->queue, pfs->channel, 0, temp1, 0)) != 0) {
        return ret;
    }
    if ((ret = func_80027330(pfs->queue, pfs->channel, 0, temp2)) != 0) {
        return ret;
    }
    if (func_800261B0(temp1, temp2, 32) != 0) {
        return 11;
    }
    return func_80027520(pfs->queue, pfs->channel, 0, save, 0);
}

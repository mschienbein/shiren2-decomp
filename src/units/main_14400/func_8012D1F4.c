#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

/* Audio sample DMA cache line: used/free list links, age, ROM start, RAM buffer. */
typedef struct DmaBuffer8012D1F4 {
    struct DmaBuffer8012D1F4 *prev;
    struct DmaBuffer8012D1F4 *next;
    s32 frames;
    u32 startAddr;
    u8 *ptr;
} DmaBuffer8012D1F4;

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

typedef struct OSMesgQueue8012D1F4 OSMesgQueue8012D1F4;

extern DmaBuffer8012D1F4 *D_801CA730;
extern DmaBuffer8012D1F4 *D_801CA734;
extern OSIoMesg *D_801CA73C;
extern u32 D_801CA744;
extern s32 D_801CA748;
extern OSMesgQueue8012D1F4 D_801CA74C;
extern void *D_801CA764;
extern void *D_801D2C28;
extern u32 D_801DE978;

extern s32 func_800299E0(void *pihandle, OSIoMesg *mb, s32 direction);

/* Find or start loading the cache line holding [addr, addr + len). */
DmaBuffer8012D1F4 *func_8012D1F4(u32 addr, s32 len) {
    void *handle;
    DmaBuffer8012D1F4 *dma;
    DmaBuffer8012D1F4 *last;
    DmaBuffer8012D1F4 *buf;
    u32 end;
    OSIoMesg *msg;

    if ((addr & 0xFF000000) == 0xFF000000) {
        handle = D_801D2C28;
        addr &= 0xFFFFFF;
        addr += 0x140000;
    } else {
        if (D_801DE978 & 1) {
            return 0;
        }
        handle = D_801CA764;
    }
    last = 0;
    dma = D_801CA730;
    end = addr + len;
    while (dma != 0) {
        if (addr < dma->startAddr) {
            break;
        }
        if (end <= dma->startAddr + D_801CA744) {
            dma->frames = 2;
            return dma;
        }
        last = dma;
        dma = dma->next;
    }
    buf = D_801CA734;
    if (buf == 0) {
        return D_801CA730;
    }
    D_801CA734 = buf->next;
    if (last != 0) {
        DmaBuffer8012D1F4 *next = last->next;

        buf->next = next;
        if (next != 0) {
            next->prev = buf;
        }
        buf->prev = last;
        last->next = buf;
    } else {
        DmaBuffer8012D1F4 *head = D_801CA730;

        buf->prev = 0;
        buf->next = head;
        if (head != 0) {
            head->prev = buf;
        }
        D_801CA730 = buf;
    }
    buf->startAddr = addr & ~1;
    buf->frames = 2;
    msg = &D_801CA73C[D_801CA748++];
    msg->hdr.pri = 0;
    msg->hdr.retQueue = &D_801CA74C;
    msg->dramAddr = buf->ptr;
    msg->devAddr = buf->startAddr;
    msg->size = D_801CA744;
    func_800299E0(handle, msg, 0);
    return buf;
}

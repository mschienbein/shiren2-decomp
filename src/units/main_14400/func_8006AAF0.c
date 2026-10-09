#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef void *Message;
typedef struct { void *receiveWaiters; void *sendWaiters; long validCount; long first; long capacity; Message *messages; } Queue;
typedef struct { u16 type; u8 priority; Queue *returnQueue; void *dramAddr; u32 devAddr; u32 size; void *piHandle; } OSIoMesg;
extern s32 D_8013CA20;
extern void *D_8016FD70;
extern void func_80027EA0(Queue *queue, Message *messages, long capacity);
extern void func_8002B000(void *address, s32 length);
extern s32 func_800299E0(void *handle, OSIoMesg *message, s32 direction);
extern long func_8002FEA0(Queue *queue, Message *message, long flags);

void func_8006AAF0(void *dst, u32 devAddr, s32 size)
{
    OSIoMesg request;
    Queue queue;
    Message storage;
    Message reply;
    s32 chunk;
    s32 remaining;
    u8 *cursor;
    u32 source;
    if (D_8013CA20 != 0) {
        source = devAddr;
        cursor = dst;
        remaining = (size + 1) & ~1;
        func_80027EA0(&queue, &storage, 1);
        request.priority = 0;
        request.returnQueue = &queue;
        func_8002B000(cursor, remaining);
        while (remaining != 0) {
            chunk = remaining;
            if (chunk > 0x1000)
                chunk = 0x1000;
            request.devAddr = source;
            request.dramAddr = cursor;
            request.size = chunk;
            func_800299E0(D_8016FD70, &request, 0);
            func_8002FEA0(&queue, &reply, 1);
            source += chunk;
            cursor += chunk;
            remaining -= chunk;
        }
    }
}

#include "common.h"

typedef unsigned char u8;
/* Local view of the resident message queue (six words). */
typedef struct { s32 field_00[6]; } Queue;
typedef struct {
    unsigned short field_00;
    u8 field_02;
    u8 field_03;
    Queue *field_04;
    void *field_08; /* RAM address */
    u32 field_0C;   /* PI device address */
    u32 field_10;   /* length */
    void *field_14;
} Request;
extern void *D_80138B08;
/* The resident queue family uses signed long counts and results. */
extern void func_80027EA0(Queue *queue, void **messages, signed long capacity);
extern void func_8002B000(void *address, s32 length);
extern s32 func_800299E0(void *, Request *, s32);
extern signed long func_8002FEA0(Queue *queue, void **message, signed long flags);
extern s32 func_80083D8C(void *, void *, u32);
extern void *func_80032D94(void *dst, const void *src, u32 count);

/* Reads length bytes from a PI device address in verified 0x40-byte chunks. */
void func_80043C2C(u32 deviceAddress, void *destination, u32 length) {
    u8 buffer[0x100];
    Request request;
    Queue queue;
    void *message;
    void *received;
    func_80027EA0(&queue, &message, 1);
    request.field_02 = 0;
    request.field_04 = &queue;
    for (;;) {
        u32 size;
        s32 retry;
        if (!length) break;
        size = length;
        if (size > 0x40) size = 0x40;
        request.field_08 = destination;
        request.field_0C = deviceAddress;
        request.field_10 = size;
        func_8002B000(destination, (s32)size);
        func_800299E0(D_80138B08, &request, 0);
        func_8002FEA0(&queue, &received, 1);
        for (retry = 9; retry >= 0; retry--) {
            request.field_08 = buffer;
            request.field_0C = deviceAddress;
            request.field_10 = size;
            func_8002B000(buffer, (s32)size);
            if (!func_800299E0(D_80138B08, &request, 0)) {
                func_8002FEA0(&queue, &received, 1);
                if (!func_80083D8C(destination, buffer, size)) break;
                func_80032D94(destination, buffer, size);
            }
        }
        length -= size;
        destination = (u8 *)destination + size;
        deviceAddress += size;
    }
}

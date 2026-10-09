#include "common.h"
#include "controller_queue_view.h"
#include "pi_handle_view.h"
typedef ControllerQueueView MessageQueue;
typedef struct {
    unsigned short field_00; unsigned char field_02, field_03;
    MessageQueue *field_04; void *field_08; u32 field_0c, field_10; void *field_14;
} IoMessage;
extern PiHandleView *D_80138B08;
extern void func_80027EA0(MessageQueue *, void **, ControllerQueueS32);
extern s32 func_800299E0(PiHandleView *, IoMessage *, s32);
extern void func_80034720(void *, s32);
extern void func_8002B000(void *, s32);
extern ControllerQueueS32 func_8002FEA0(MessageQueue *, void **, ControllerQueueS32);
extern s32 func_80083D8C(void *, void *, u32);
void func_80043D74(u32 address, unsigned char *data, u32 length) {
    unsigned char buffer[0x100];
    IoMessage io;
    MessageQueue queue;
    void *queue_storage;
    void *received;
    u32 chunk;
    s32 retry;
    func_80027EA0(&queue, &queue_storage, 1);
    io.field_02 = 0;
    io.field_04 = &queue;
    for (;;) {
        if (length == 0) return;
        chunk = length;
        if (chunk > 0x40) chunk = 0x40;
        for (retry = 9; retry >= 0; retry--) {
            io.field_08 = data;
            io.field_0c = address;
            io.field_10 = chunk;
            func_80034720(data, (s32)chunk);
            if (func_800299E0(D_80138B08, &io, 1) == 0) {
                func_8002FEA0(&queue, &received, 1);
                io.field_08 = buffer;
                io.field_0c = address;
                io.field_10 = chunk;
                func_8002B000(buffer, (s32)chunk);
                if (func_800299E0(D_80138B08, &io, 0) == 0) {
                    func_8002FEA0(&queue, &received, 1);
                    if (func_80083D8C(data, buffer, chunk) == 0) break;
                }
            }
        }
        length -= chunk;
        data += chunk;
        address += chunk;
    }
}

#include "common.h"

typedef unsigned char u8;
typedef struct {
    void *receive;
    void *send;
    long valid, first, capacity;
    void **messages;
} Queue;
extern void func_80033048(const char *format, ...);
extern void func_80027EA0(Queue *queue, void **messages, long capacity);
extern void func_80031E90(s32 event, Queue *queue, void *message);
extern long func_8002FEA0(Queue *queue, void **message, long flags);
extern void func_8006CD14(void);
extern u8 D_801A6C70;

/* The OS thread creation ABI supplies an unused argument. */
void func_8006D080(void *argument) {
    Queue queue;
    void *messages[8];
    func_80033048("OO FAULT THREAD START...\n");
    func_80027EA0(&queue, messages, 8);
    func_80031E90(12, &queue, (void *)1);
    for (;;) {
        func_8002FEA0(&queue, 0, 1);
    }
}

/* Separate 64-byte entry at 0x8006D0CC, after the thread's endless receive loop (its own
 * catalogue entry after the boundary revision). No direct J/JAL, address materialization or data
 * word targets it in the four mapped CPU images; func_8006D10C reads the same D_801A6C70 byte. */
u8 func_8006D0CC(s32 value) {
    u8 old = D_801A6C70;
    func_8006CD14();
    D_801A6C70 = value;
    return old;
}

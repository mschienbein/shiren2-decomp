#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef signed char s8;
typedef short s16;

typedef struct {
    void *receive_waiters;
    void *send_waiters;
    long valid_count;
    long first;
    long capacity;
    void **messages;
} MesgQueue80072CE4;
extern MesgQueue80072CE4 D_801A71CC;
extern void *D_801A71E8;
extern s32 D_801A71C4;
void func_80027EA0(MesgQueue80072CE4 *queue, void **msgs, long count);
long func_80031D50(MesgQueue80072CE4 *queue, void *message, long flags);
long func_8002FEA0(MesgQueue80072CE4 *queue, void **msg, long flags);
void func_80027FA0(void *thread);
void func_8006E740(s32 value);
void func_80060C54(u32 value);

void func_80072CE4(void) {
    MesgQueue80072CE4 queue;
    void *msg;

    func_80027EA0(&queue, &msg, 1);
    func_80031D50(&D_801A71CC, &queue, 1);
    func_8002FEA0(&queue, 0, 1);
    func_80027FA0(D_801A71E8);
    func_8006E740(1);
    func_80060C54(3);
    D_801A71C4 = 0;
}

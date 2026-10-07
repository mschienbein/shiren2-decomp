#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;

/* Complete 24-byte resident queue layout, with long-typed queue counters. */
typedef struct {
    void *receive_waiters;
    void *send_waiters;
    long valid_count;
    long first;
    long capacity;
    void **messages;
} MesgQueue;
extern s32 D_801652E8;
extern MesgQueue *D_801652EC[5];
u32 func_80031F90(u32 mask);
void func_80027EA0(MesgQueue *mq, void **msg, long count);
long func_8002FEA0(MesgQueue *mq, void **msg, long flags);
void func_80058A4C(void) {
    MesgQueue mq;
    void *msg;
    s32 i;
    u32 mask = func_80031F90(1);
    if (D_801652E8 != 0) {
        func_80027EA0(&mq, &msg, 1);
        for (i = 0; i < 5; i++) {
            if (D_801652EC[i] == 0) break;
        }
        if (i < 5) {
            D_801652EC[i] = &mq;
            func_80031F90(mask);
            func_8002FEA0(&mq, 0, 1);
        }
    }
    func_80031F90(mask);
}

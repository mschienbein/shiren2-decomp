#include "common.h"
typedef unsigned char u8;
typedef unsigned long long OSTime;
typedef void *ProbeMessage;
struct ProbeThread;
typedef struct {
    struct ProbeThread *receive_waiters, *send_waiters;
    long valid_count, first, capacity;
    ProbeMessage *messages;
} ProbeMessageQueue;
typedef struct {
    s32 count_0;
    s32 field_4;
    OSTime time_8;
    u32 counts_10[8];
    OSTime timing_30[8][4][2];
} Frame;
typedef struct {
    s32 message_0, message_4;
    ProbeMessageQueue queue_8;
    u8 pad_20[0x9D0];
    s32 frames_9F0;
    u8 rate_9F4, divisor_9F5, field_9F6, flags_9F7;
} Worker;
extern const char D_8014C6B8[];
extern u8 D_801A6C71, D_801A6C72;
extern s32 D_801A6C74;
extern Frame D_801A6C78[2];
extern Frame *D_801A70D8, *D_801A70DC;
extern void func_80033048(const char *fmt,...);
extern long func_8002FEA0(ProbeMessageQueue *queue, ProbeMessage *message, long flags);
extern void func_8006C7B4(u8 mask, void *message);
extern OSTime func_8002AAB0(void);
extern OSTime func_80036510(OSTime value, OSTime divisor);
extern s32 func_80025E80(void);
extern void func_80034370(float value);
extern void func_80033CC0(u8 active);
void func_8006C580(Worker *self) {
    ProbeMessage message;
    s32 countdown=0;
    func_80033048(D_8014C6B8);
    self->frames_9F0=0;
    for (;;) {
        func_8002FEA0(&self->queue_8,&message,1);
        switch ((u32)message) {
        case 0: {
            s32 count;
            ++self->frames_9F0;
            func_8006C7B4(1,self);
            count=++D_801A6C74;
            if (D_801A6C72) {
                s32 previous;
                D_801A6C74=0;
                D_801A6C72=0;
                D_801A70DC->count_0=count;
                previous=D_801A6C71;
                D_801A6C71=(previous+1)&1;
                D_801A70D8=&D_801A6C78[previous];
                D_801A70DC=&D_801A6C78[D_801A6C71];
                {
                    u32 i,j;
                    D_801A70DC->time_8=func_80036510(func_8002AAB0()<<6,3000);
                    for (i=0;i<8;i++) {
                        D_801A70DC->counts_10[i]=0;
                        for (j=0;j<4;j++) {
                            D_801A70DC->timing_30[i][j][0]=0;
                            D_801A70DC->timing_30[i][j][1]=0;
                        }
                    }
                }
            }
            if (self->flags_9F7) {
                if (countdown) --countdown;
                else {
                    self->flags_9F7|=2;
                    func_80025E80();
                    func_80034370(1.0f);
                    func_80033CC0(1);
                }
            }
            break;
        }
        case 1:
            self->flags_9F7|=1;
            func_8006C7B4(2,&self->message_4);
            countdown=(s32)(self->rate_9F4>>1)/(s32)self->divisor_9F5-3;
            break;
        }
    }
}

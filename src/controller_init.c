/* Independent two-function reconstruction of JP ROM 0x2D10..0x2F60.
 * Real extern addresses and the initialized word are resolved by the private link.
 * Pinned SDK files supply semantic/type references; no SDK headers are included.
 */
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned long u32;
typedef signed long s32;
typedef unsigned long long u64;
typedef void *ProbeMessage;

struct ProbeThread;
typedef struct {
    struct ProbeThread *receive_waiters;
    struct ProbeThread *send_waiters;
    s32 valid_count;
    s32 first;
    s32 capacity;
    ProbeMessage *messages;
} ProbeMessageQueue;

typedef struct ProbeTimer {
    struct ProbeTimer *next;
    struct ProbeTimer *previous;
    u64 interval;
    u64 remaining;
    ProbeMessageQueue *queue;
    ProbeMessage message;
} ProbeTimer;

#include "controller_status.h"

typedef struct {
    u8 reserved0;
    u8 tx_count;
    u8 rx_count;
    u8 command;
    u8 type_low;
    u8 type_high;
    u8 status;
    u8 reserved7;
} ProbeControllerResponse;

typedef struct {
    u32 words[15];
    u32 command_status;
} ProbePifRam __attribute__((aligned(16)));

typedef char probe_int32[(sizeof(int) == 4) ? 1 : -1];
typedef char probe_pointer32[(sizeof(void *) == 4) ? 1 : -1];
typedef char probe_long32[(sizeof(u32) == 4) ? 1 : -1];
typedef char probe_time64[(sizeof(u64) == 8 && __alignof__(u64) == 8) ? 1 : -1];
typedef char probe_short16[(sizeof(u16) == 2) ? 1 : -1];
typedef char probe_queue_layout[(sizeof(ProbeMessageQueue) == 0x18 && __alignof__(ProbeMessageQueue) == 4) ? 1 : -1];
typedef char probe_timer_layout[(sizeof(ProbeTimer) == 0x20 && __alignof__(ProbeTimer) == 8) ? 1 : -1];
typedef char probe_status_layout[(sizeof(ProbeControllerStatus) == 4 && __alignof__(ProbeControllerStatus) == 2) ? 1 : -1];
typedef char probe_response_layout[(sizeof(ProbeControllerResponse) == 8 && __alignof__(ProbeControllerResponse) == 1) ? 1 : -1];
typedef char probe_pif_layout[(sizeof(ProbePifRam) == 64 && __alignof__(ProbePifRam) == 16) ? 1 : -1];

extern u8 D_80039008;
extern u8 D_80039018;
extern ProbeMessage D_8003901C;
extern ProbePifRam D_80040FD0;
extern ProbeMessageQueue D_80041390;

/* Original initialized four-byte state, followed by assembler data padding. */
s32 D_80036F80 = 0;

extern u64 func_8002AAB0(void);
extern void func_80027EA0(ProbeMessageQueue *, ProbeMessage *, s32);
extern int func_80032110(ProbeTimer *, u64, u64, ProbeMessageQueue *, ProbeMessage);
extern s32 func_8002FEA0(ProbeMessageQueue *, ProbeMessage *, s32);
extern void func_80027B60(u8);
extern int func_80032500(int, void *);
extern void func_80032270(void);
void func_80027AAC(u8 *, ProbeControllerStatus *);

s32 func_80027910(ProbeMessageQueue *queue, u8 *found_channels,
                  ProbeControllerStatus *output)
{
    ProbeMessage message;
    s32 result = 0;
    u64 now;
    ProbeTimer timer;
    ProbeMessageQueue wait_queue;

    if (D_80036F80) {
        return 0;
    }
    D_80036F80 = 1;

    now = func_8002AAB0();
    if (now < 0x0165A0BCULL) {
        func_80027EA0(&wait_queue, &message, 1);
        func_80032110(&timer, 0x0165A0BCULL - now, 0, &wait_queue, &message);
        func_8002FEA0(&wait_queue, &message, 1);
    }

    D_80039008 = 4;
    func_80027B60(0);
    result = func_80032500(1, D_80040FD0.words);
    func_8002FEA0(queue, &message, 1);
    result = func_80032500(0, D_80040FD0.words);
    func_8002FEA0(queue, &message, 1);

    func_80027AAC(found_channels, output);
    D_80039018 = 0;
    func_80032270();
    func_80027EA0(&D_80041390, &D_8003901C, 1);
    return result;
}

void func_80027AAC(u8 *found_channels, ProbeControllerStatus *output)
{
    u8 *cursor;
    ProbeControllerResponse response;
    int channel;
    u8 valid_channels = 0;

    cursor = (u8 *)&D_80040FD0.words[0];
    for (channel = 0; channel < D_80039008;
         channel++, cursor += sizeof(response), output++) {
        response = *(ProbeControllerResponse *)cursor;
        output->error = (response.rx_count & 0xC0) >> 4;
        if (output->error != 0) {
            continue;
        }

        output->type = response.type_high << 8 | response.type_low;
        output->status = response.status;
        valid_channels |= 1 << channel;
    }
    *found_channels = valid_channels;
}

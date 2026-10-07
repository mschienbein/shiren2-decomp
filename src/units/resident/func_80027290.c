/* Independent reconstruction; original address-based symbols are retained. */
typedef unsigned char u8;
typedef unsigned long u32;
typedef signed long s32;
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

typedef struct {
    u32 words[15];
    u32 command_status;
} ProbePifRam __attribute__((aligned(16)));

typedef char word_width[(sizeof(u32) == 4 && sizeof(s32) == 4) ? 1 : -1];
typedef char abi_width[(sizeof(int) == 4 && sizeof(void *) == 4) ? 1 : -1];
typedef char queue_layout[(sizeof(ProbeMessageQueue) == 24 && __alignof__(ProbeMessageQueue) == 4) ? 1 : -1];
typedef char pif_layout[(sizeof(ProbePifRam) == 64 && __alignof__(ProbePifRam) == 16) ? 1 : -1];

extern u8 D_80039018;
extern ProbePifRam D_80040FD0;
extern void func_800322C4(void);
extern void func_80027B60(u8 command);
extern int func_80032500(int direction, void *buffer);
extern s32 func_8002FEA0(ProbeMessageQueue *queue, ProbeMessage *message, s32 flags);
extern void func_80032330(void);

s32 func_80027290(ProbeMessageQueue *queue)
{
    s32 result;

    func_800322C4();
    if (D_80039018 != 0) {
        func_80027B60(0);
        func_80032500(1, D_80040FD0.words);
        func_8002FEA0(queue, (ProbeMessage *)0, 1);
    }

    result = func_80032500(0, D_80040FD0.words);
    D_80039018 = 0;
    func_80032330();
    return result;
}

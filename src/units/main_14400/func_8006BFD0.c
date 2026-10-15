#include "common.h"
#include "vi_mode.h"

typedef unsigned char u8;
typedef struct { void *receive, *send; long valid, first, capacity; void **messages; } MessageQueue;

typedef struct {
    unsigned long long fields_00[0x1B0 / 8];
} Thread;

typedef struct {
    u32 fields_00[0x230 / 4];
} FrameBuffer;

/* +0x9F5..+0x9FB: the retrace interval (read back at 0x8006C188 as the third argument
   of func_80034150, the osViSetEvent-shaped call) and six independent byte fields
   (including counters); the same fields as the shared SchedulerState view's
   retrace_interval/flags9F6..flags9FB.
   The ROM initializes them through one base pointer (s0 = D_801D85D0+0x9F5 at
   0x8006C02C; sb 0..6($s0) at 0x8006C048..0x8006C098), hence the grouped view. */
typedef struct {
    unsigned char retrace_interval;
    unsigned char flags9F6;
    unsigned char flags9F7;
    unsigned char flags9F8;
    unsigned char flags9F9;
    unsigned char flags9FA;
    unsigned char flags9FB;
} SchedulerTail;
typedef struct {
    s32 field_000;
    s32 field_004;
    MessageQueue queue_008;
    void *messages_020[8];
    MessageQueue queue_040;
    void *messages_058[8];
    MessageQueue queue_078;
    void *messages_090[8];
    MessageQueue queue_0B0;
    void *messages_0C8[8];
    MessageQueue queue_0E8;
    void *messages_100[1];
    MessageQueue queue_104;
    void *messages_11C[4];
    MessageQueue queue_12C;
    void *messages_144[8];
    s32 field_164;
    Thread threads_168[5];
    void *field_9D8;
    void *field_9DC;
    void *field_9E0;
    void *field_9E4;
    void *field_9E8;
    s32 (*field_9EC)(void);
    s32 field_9F0;
    unsigned char rate_9F4;
    SchedulerTail tail_9F5;                  /* +0x9F5 */
} Scheduler;

extern s32 D_80000300;
/* Retrace-worker stack, ending at the next worker's 0x801A0C70 base. */
extern u8 D_8019FC70[0x1000];
extern u8 D_801A0C70[0x2000];
extern u8 D_801A2C70[0x2000];
extern u8 D_801A4C70[0x1000];
extern u8 D_801A5C70[0x1000];
extern unsigned char D_801A6C70;
/* Index of the framebuffer being drawn (0/1); toggled after the reset below. */
extern unsigned char D_801A6C71;
extern unsigned char D_801A6C72;
extern FrameBuffer D_801A6C78[2];
extern FrameBuffer *D_801A70D8;
extern FrameBuffer *D_801A70DC;
extern void *D_801D85AC[];
extern Scheduler D_801D85D0;
extern MessageQueue D_801E00D4;
extern void func_8006D080(void *);
extern void func_8006C580(void *);
extern void func_8006C81C(void *);
extern void func_8006C9B8(void *);
extern void func_8006CDA8(void *);
extern u8 *func_8006A810(u8 *, s32, s32);
extern void func_80027EA0(MessageQueue *, void **, long);
extern void func_80033DB0(s32);
extern void func_80033CC0(u8);
extern void func_80034150(MessageQueue *, void *, u32);
extern void func_80031E90(s32, MessageQueue *, void *);
extern void func_80058E8C(void);
extern void func_800585B0(void);
extern void func_80027ED0(Thread *, s32, void (*)(void *), void *, void *, s32);
extern void func_80032B50(Thread *);

void func_8006BFD0(s32 mode) {
    SchedulerTail *tail;
    FrameBuffer *buffers;
    MessageQueue *queue0;
    MessageQueue *queue1;
    MessageQueue *queue2;
    OSViMode *video_mode;
    if (D_80000300 == 0) {
        D_801D85D0.rate_9F4 = 50;
    } else {
        D_801D85D0.rate_9F4 = 60;
    }
    tail = &D_801D85D0.tail_9F5;
    buffers = D_801A6C78;
    D_801A6C71 = 0;
    tail->retrace_interval = 1;
    tail->flags9F6 = 1;
    tail->flags9F7 = 0;
    D_801D85D0.field_000 = 0;
    D_801D85D0.field_004 = 1;
    D_801D85D0.field_9DC = 0;
    D_801D85D0.field_9E0 = 0;
    D_801D85D0.field_9E4 = 0;
    D_801D85D0.field_9E8 = 0;
    D_801D85D0.field_9EC = 0;
    D_801D85D0.field_9D8 = 0;
    tail->flags9F8 = 0;
    tail->flags9F9 = 0;
    tail->flags9FA = 0;
    tail->flags9FB = 0;
    D_801A6C70 = 0;
    D_801A6C71 ^= 1;
    D_801A6C72 = 1;
    D_801A70D8 = buffers;
    D_801A70DC = &buffers[D_801A6C71];
    func_8006A810((u8 *)&buffers[0], 0, sizeof(FrameBuffer));
    func_8006A810((u8 *)&buffers[1], 0, sizeof(FrameBuffer));
    queue0 = &D_801D85D0.queue_008;
    func_80027EA0(queue0, D_801D85D0.messages_020, 8);
    queue1 = &D_801D85D0.queue_040;
    func_80027EA0(queue1, D_801D85D0.messages_058, 8);
    queue2 = &D_801D85D0.queue_078;
    func_80027EA0(queue2, D_801D85D0.messages_090, 8);
    func_80027EA0(&D_801D85D0.queue_104, D_801D85D0.messages_11C, 4);
    func_80027EA0(&D_801D85D0.queue_0E8, D_801D85D0.messages_100, 1);
    func_80027EA0(&D_801D85D0.queue_0B0, D_801D85D0.messages_0C8, 8);
    func_80027EA0(&D_801D85D0.queue_12C, D_801D85D0.messages_144, 8);
    func_80033DB0(0xFE);
    video_mode = &D_800374E0[(u8)mode];
    func_800341B0(video_mode);
    func_80033CC0(1);
    func_80034150(queue0, 0, tail->retrace_interval);
    func_80031E90(0xE, queue0, (void *)1);
    func_80031E90(4, queue1, (void *)2);
    func_80031E90(9, queue2, (void *)3);
    func_80027EA0(&D_801E00D4, D_801D85AC, 8);
    func_80031E90(5, &D_801E00D4, (void *)1);
    func_80033CC0(1);
    func_80058E8C();
    func_800341B0(video_mode);
    func_80033CC0(1);
    func_800585B0();
    func_80027ED0(&D_801D85D0.threads_168[4], 0x1F8, func_8006D080, &D_801D85D0, D_801A5C70 + sizeof(D_801A5C70), 0x7A);
    func_80032B50(&D_801D85D0.threads_168[4]);
    func_80027ED0(&D_801D85D0.threads_168[0], 0x1F4, func_8006C580, &D_801D85D0, D_8019FC70 + sizeof(D_8019FC70), 0x78);
    func_80032B50(&D_801D85D0.threads_168[0]);
    func_80027ED0(&D_801D85D0.threads_168[1], 0x1F5, func_8006C81C, &D_801D85D0, D_801A0C70 + sizeof(D_801A0C70), 0x69);
    func_80032B50(&D_801D85D0.threads_168[1]);
    func_80027ED0(&D_801D85D0.threads_168[2], 0x1F6, func_8006C9B8, &D_801D85D0, D_801A2C70 + sizeof(D_801A2C70), 0x64);
    func_80032B50(&D_801D85D0.threads_168[2]);
    func_80027ED0(&D_801D85D0.threads_168[3], 0x1F7, func_8006CDA8, &D_801D85D0, D_801A4C70 + sizeof(D_801A4C70), 0x6E);
    func_80032B50(&D_801D85D0.threads_168[3]);
}

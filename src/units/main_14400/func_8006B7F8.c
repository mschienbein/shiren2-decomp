#include "common.h"

typedef unsigned char u8;

typedef struct Gfx {
    u32 command;
    u32 value;
} Gfx;

/* RSP microcode (text, data) pair. */
typedef struct Ucode8006B7F8 {
    void *text;
    void *data;
} Ucode8006B7F8;

/* 0x58-byte scheduler task record; an OSTask occupies 0x10..0x4F. */
typedef struct TaskRecord8006B7F8 {
    u8 state_00;
    u8 kind_01;
    u8 pad_02[2];
    void *framebuffer_04;
    void *depthbuffer_08;
    u8 pad_0C[0x20 - 0xC];
    void *ucode_20;
    u8 pad_24[4];
    void *ucode_data_28;
    u8 pad_2C[0x40 - 0x2C];
    Gfx *data_ptr_40;
    s32 data_size_44;
    u8 pad_48[0x50 - 0x48];
    void *completion_queue_50;
    void *completion_message_54;
} TaskRecord8006B7F8;

typedef struct TaskMsg8006B7F8 {
    s32 type;
    TaskRecord8006B7F8 *task;
} TaskMsg8006B7F8;

/* Owned .data byte (original value 0): next free task slot of the 20-entry ring. */
u8 D_8013CA40 = 0;
extern Ucode8006B7F8 D_8013CA44[];
extern TaskRecord8006B7F8 D_80199138[20];
extern TaskMsg8006B7F8 D_80199818[20];

void func_800347A0(void);
void func_8006CC58(void *msg);

void func_8006B7F8(Gfx *dl, s32 size, s32 ucode, s32 kind, void *framebuffer, void *depthbuffer, void *completion_queue, void *completion_message) {
    s32 slot = D_8013CA40;
    TaskRecord8006B7F8 *task = &D_80199138[slot];
    TaskMsg8006B7F8 *msg;

    task->data_ptr_40 = dl;
    task->data_size_44 = size;
    task->ucode_20 = D_8013CA44[ucode].text;
    task->ucode_data_28 = D_8013CA44[ucode].data;
    task->state_00 = 0;
    task->kind_01 = kind;
    task->framebuffer_04 = framebuffer;
    task->depthbuffer_08 = depthbuffer;
    task->completion_queue_50 = completion_queue;
    task->completion_message_54 = completion_message;
    msg = &D_80199818[slot];
    msg->type = 0xA;
    msg->task = task;
    D_8013CA40 = (D_8013CA40 + 1) % 20;
    func_800347A0();
    func_8006CC58(msg);
}

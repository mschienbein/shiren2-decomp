#include "common.h"

typedef unsigned char u8;
/* Caller-supplied audio microcode request: data list and the ucode to run. */
typedef struct { void *data; s32 size; void *ucode; void *ucode_data; } Args;
typedef struct Queue Queue;
/* Complete 0x58-byte scheduler task at D_801615E0 (header, OSTask fields,
 * reply queue and message). */
typedef struct {
    u8 field_00, pad_01[15];
    s32 type, flags;
    const void *boot;
    s32 boot_size;
    void *ucode;
    s32 ucode_size;
    void *ucode_data;
    s32 ucode_data_size;
    void *dram;
    s32 dram_size;
    void *output;
    void *output_size;
    void *data;
    s32 data_size;
    void *yield;
    s32 yield_size;
    Queue *queue;
    void *message;
} Task;
typedef struct { s32 kind; Task *task; } Request;

extern Task D_801615E0;
extern Queue D_80161684;
extern const u8 D_80136A10[], D_80136AE0[];
extern Request D_80161638;
void func_80052BB0(void);
void func_8006C990(void *msg);
long func_8002FEA0(void *queue, void **message, long flags);

/* Builds the audio RSP task, posts it to the scheduler and waits for completion. */
void func_80052DA4(Args *args) {
    void *ucode_data;

    func_80052BB0();
    D_801615E0.data = args->data;
    D_801615E0.data_size = args->size;
    D_801615E0.ucode = args->ucode;
    ucode_data = args->ucode_data;
    D_801615E0.field_00 = 1;
    D_801615E0.queue = &D_80161684;
    D_801615E0.type = 2;
    /* local-arithmetic-qualification: linker boundary addresses delimit the
       boot microcode; ordinary pointer subtraction needs one C array object. */
    D_801615E0.boot_size = (u32)D_80136AE0 - (u32)D_80136A10;
    D_801615E0.ucode_size = 0x1000;
    D_801615E0.ucode_data_size = 0x800;
    D_801615E0.message = 0;
    D_801615E0.boot = D_80136A10;
    D_801615E0.flags = 0;
    D_801615E0.dram = 0;
    D_801615E0.dram_size = 0;
    D_801615E0.output = 0;
    D_801615E0.output_size = 0;
    D_801615E0.yield = 0;
    D_801615E0.yield_size = 0;
    D_801615E0.ucode_data = ucode_data;
    D_80161638.kind = 10;
    D_80161638.task = &D_801615E0;
    func_8006C990(&D_80161638);
    func_8002FEA0(&D_80161684, 0, 1);
}

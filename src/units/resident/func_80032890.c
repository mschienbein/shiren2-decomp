#include "common.h"

/* libultra sptask.c: osSpTaskLoad, osSpTaskStartGo */

typedef unsigned long long u64;

#define PHYS_TO_K1(x) ((u32)(x) | 0xA0000000)
#define IO_READ(addr) (*(volatile u32 *)PHYS_TO_K1(addr))

typedef struct {
    u32 type;
    u32 flags;
    u64 *ucode_boot;
    u32 ucode_boot_size;
    u64 *ucode;
    u32 ucode_size;
    u64 *ucode_data;
    u32 ucode_data_size;
    u64 *dram_stack;
    u32 dram_stack_size;
    u64 *output_buff;
    u64 *output_buff_size;
    u64 *data_ptr;
    u32 data_size;
    u64 *yield_data_ptr;
    u32 yield_data_size;
} OSTask_t;

typedef union {
    OSTask_t t;
    long long force_structure_alignment;
} OSTask;

#define OS_TASK_YIELDED 0x1
#define OS_TASK_LOADABLE 0x4
#define OS_YIELD_DATA_SIZE 0xC00
#define SP_IMEM_START 0x04001000

#define _osVirtualToPhysical(ptr) \
    if (ptr != 0) { \
        ptr = (void *)func_800340F0(ptr); \
    }

extern OSTask D_8003EBC0; /* tmp_task */

void *func_800262C0(const void *src, void *dst, s32 len);
u32 func_800340F0(void *p);
void func_80034720(void *p, s32 len);
void func_80032880(u32 status);
s32 func_80032850(u32 pc);
s32 func_80032730(s32 direction, u32 devAddr, void *dramAddr, u32 size);
s32 func_80032700(void);

void func_80032890(OSTask *intp)
{
    OSTask *tp = &D_8003EBC0;

    func_800262C0(intp, tp, sizeof(OSTask));
    _osVirtualToPhysical(tp->t.ucode);
    _osVirtualToPhysical(tp->t.ucode_data);
    _osVirtualToPhysical(tp->t.dram_stack);
    _osVirtualToPhysical(tp->t.output_buff);
    _osVirtualToPhysical(tp->t.output_buff_size);
    _osVirtualToPhysical(tp->t.data_ptr);
    _osVirtualToPhysical(tp->t.yield_data_ptr);

    if (tp->t.flags & OS_TASK_YIELDED) {
        tp->t.ucode_data = tp->t.yield_data_ptr;
        tp->t.ucode_data_size = tp->t.yield_data_size;
        intp->t.flags &= ~OS_TASK_YIELDED;
        if (tp->t.flags & OS_TASK_LOADABLE) {
            tp->t.ucode = (u64 *)IO_READ((u32)intp->t.yield_data_ptr + OS_YIELD_DATA_SIZE - 4);
        }
    }
    func_80034720(tp, sizeof(OSTask));
    func_80032880(0x2B00);
    while (func_80032850(SP_IMEM_START) == -1) {
    }
    while (func_80032730(1, SP_IMEM_START - sizeof(*tp), tp, sizeof(OSTask)) == -1) {
    }
    while (func_80032700()) {
    }
    while (func_80032730(1, SP_IMEM_START, tp->t.ucode_boot, tp->t.ucode_boot_size) == -1) {
    }
}

void func_80032A9C(OSTask *tp)
{
    while (func_80032700()) {
    }
    func_80032880(0x125);
}

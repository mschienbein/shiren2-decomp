#include "common.h"
#include "controller_queue_view.h"
/* Request from func_8012D688 (via D_80148AAC->x8): command list, its size, ucode and
 * ucode data addresses. */
typedef struct { void *data_ptr; s32 data_size; void *ucode; void *ucode_data; } Input;
/* OSScTask-shaped scheduler task (next/state/flags/framebuffer, the OSTask list,
 * msgQ/msg) plus four trailing scheduler words; 0x68 bytes on the stack. */
typedef struct {
    void *next; s32 state, flags; void *framebuffer;
    s32 type, task_flags;
    char *ucode_boot; s32 ucode_boot_size;
    void *ucode; s32 ucode_size;
    void *ucode_data; s32 ucode_data_size;
    void *dram_stack; s32 dram_stack_size;
    void *output_buff; void *output_buff_size;
    void *data_ptr; s32 data_size;
    void *yield_data_ptr; s32 yield_data_size;
    void *msgQ, *msg; s32 field_58, field_5C, field_60, field_64;
} Task;
extern char D_80136A10[], D_80136AE0[];
extern void *D_801CA770;
extern char *D_801CA774;
extern void *func_80031570(void *);
extern ControllerQueueS32 func_80031D50(void *, Task *, ControllerQueueS32), func_8002FEA0(void *, void *, ControllerQueueS32);
void func_8012D474(Input *a) {
    Task task;
    s32 message[8];
    task.msg = message;
    task.next = 0;
    task.flags = 2;
    task.msgQ = D_801CA774 + 0x30;
    task.data_ptr = a->data_ptr;
    task.data_size = a->data_size;
    task.type = 2;
    task.ucode_boot = D_80136A10;
    task.ucode_boot_size = D_80136AE0 - D_80136A10;
    task.task_flags = 0;
    task.ucode = a->ucode;
    task.ucode_data = a->ucode_data;
    task.ucode_size = 0x1000;
    task.ucode_data_size = 0x800;
    task.dram_stack = 0;
    task.dram_stack_size = 0;
    task.output_buff = 0;
    task.output_buff_size = 0;
    task.yield_data_ptr = 0;
    task.yield_data_size = 0;
    func_80031D50(func_80031570(D_801CA770), &task, 1);
    func_8002FEA0(D_801CA774 + 0x30, 0, 1);
}

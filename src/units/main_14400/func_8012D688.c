#include "common.h"

/* One stereo output frame of the command list (see func_80130614). */
typedef struct {
    s32 left;
    s32 right;
} Frame;

/* Triple-buffered AI output (allocated in func_8012D550). */
typedef struct {
    void *samples;
    s32 count;
} AudioBuffer;

/* Request handed to the driver's submit method (func_8012D474): command list, its
 * size in bytes, ucode and ucode data. */
typedef struct {
    Frame *data;
    s32 size;
    void *ucode;
    void *ucodeData;
} AudioJob;

/* Audio driver method table installed by func_8012A9CC: start (func_8012D3B0),
 * wait for the next retrace (func_8012D414), submit an RSP task (func_8012D474). */
typedef struct AudioDriverTable {
    void (*start)(void);
    void (*wait)(void);
    void (*submit)(AudioJob *job);
} AudioDriverTable;

extern AudioDriverTable *D_80148AAC;
extern AudioBuffer *D_80148AB0;
extern AudioBuffer *D_801CA934;
extern Frame *D_801CA938;
extern char D_80137E70[];
extern char D_8014A480[];
extern u32 func_80025ED0(void);
extern s32 func_80025EC0(void);
/* Defined in src/hardware_ai_next_buffer.c with 32-bit long types. */
extern long func_80026000(void *buffer, unsigned long length);
extern void func_8012D0A4(void);
extern s32 func_8012D9DC(u32 addr);
extern u32 func_800340F0(void *addr);
extern Frame *func_80130614(Frame *buf, s32 *outCount, s32 *samples, s32 count);

/* Audio thread entry (func_8012D550 starts it with a null argument): each retrace,
 * queue the buffer synthesized last frame, submit the pending command list and build
 * the next one into the following AI buffer. */
void func_8012D688(void *arg) {
    u32 index = 0;
    s32 previous = 0;
    AudioJob job;
    AudioBuffer *buffer;
    Frame *end;
    s32 count;
    u32 status;
    u32 length;

    job.data = D_801CA938;
    job.ucode = D_80137E70;
    job.ucodeData = D_8014A480;
    count = 0;
    D_80148AAC->start();
    for (;;) {
        D_80148AAC->wait();
        status = func_80025ED0();
        length = (u32)func_80025EC0() >> 2;
        if (status & 0x80000000) {
            continue;
        }
        if (D_80148AB0 != 0 && previous != 0) {
            func_80026000(D_80148AB0->samples, D_80148AB0->count * 4);
        }
        if (count != 0) {
            job.size = (end - D_801CA938) * sizeof(Frame);
            D_80148AAC->submit(&job);
            func_8012D0A4();
            D_80148AB0 = buffer;
        }
        buffer = &D_801CA934[index];
        previous = count;
        buffer->count = func_8012D9DC(length);
        /* The mixer takes the buffer's physical (RSP DMA) address. */
        end = func_80130614(D_801CA938, &count, (s32 *)func_800340F0(buffer->samples), buffer->count);
        index = (index + 1) % 3;
    }
}

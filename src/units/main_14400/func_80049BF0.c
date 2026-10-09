#include "common.h"
typedef struct { void (*handler)(void *); unsigned char pad4[0xE]; unsigned short flags12; } Task;
/* Whole menu-system object; its signed display-mode byte is at +4. */
extern signed char D_80140160[];
extern u32 func_80048AC8(void);
extern unsigned short func_80084BCC(void);
extern Task *func_80084AB4(s32 index);
extern void func_80084728(void *task);
extern s32 func_80054920(void);
extern void *func_80085154(void (*handler)(void *), s32 value);
void func_80049BF0(s32 mode) {
    s32 count;
    Task *task;
    u32 status = func_80048AC8();
    status ^= 1;
    if (status) return;
    count = func_80084BCC();
    if (count > 0 && func_80084AB4(count - 1)->handler == func_80084728) return;
    if (func_80054920()) return;
    task = func_80085154(func_80084728, mode);
    {
        s32 ready = D_80140160[4];
        ready ^= 1;
        if (!ready) task->flags12 |= 0x1000;
    }
}

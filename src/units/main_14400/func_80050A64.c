#include "common.h"

/* Scheduler task view: handler at +0x0, full-word numeric command payload at +0x14. */
typedef struct { void (*handler)(void *task); char pad[0x10]; s32 value; } Task;
extern u32 D_8013968C;
extern unsigned short func_80084BCC(void);
extern Task *func_80084AB4(s32 index);
extern void *func_80085154(void (*handler)(void *), s32 value);
extern void *func_800851B0(s32 id);
extern void func_8008AFCC(void *task);
extern void func_8008AF94(void *task);
void func_80050A64(s32 value) {
    s32 count;
    s32 last;
    switch (D_8013968C) {
    case 0x126:
        count = func_80084BCC();
        last = count - 1;
        if (count > 0 && func_80084AB4(last)->handler == func_8008AFCC) {
            func_80084AB4(last)->value = value;
        } else {
            func_80085154(func_8008AFCC, value);
        }
        break;
    case 0x127:
        func_80085154(func_8008AF94, value);
        break;
    case 0x128:
        func_800851B0(value);
        break;
    }
}

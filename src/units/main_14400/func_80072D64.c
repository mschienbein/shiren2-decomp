#include "common.h"

extern unsigned char D_801A71CC[];
extern s32 D_801A720C;
extern s32 D_801A7214;
extern long func_8002FEA0(void *queue, void **message, long flags);
extern void func_8006B7AC(void);
extern void func_8005C890(s32 arg0, s32 arg1, s32 arg2);
extern long func_80031D50(void *queue, void *message, long flags);

/* func_8007268C installs this as an OS thread entry with a null, unused argument. */
void func_80072D64(void *unused) {
    void *message;

    while (func_8002FEA0(D_801A71CC, &message, 0) != 0) {
        func_8006B7AC();
    }
    func_8005C890(D_801A720C = 1, 0, D_801A7214 = 8);
    while (func_8002FEA0(D_801A71CC, 0, 0) != 0) {
        func_8006B7AC();
    }
    func_80031D50(message, 0, 0);
}

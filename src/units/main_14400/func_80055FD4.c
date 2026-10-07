#include "common.h"

typedef signed short s16;

/* One 0x60-byte record of the 32-entry task table at D_801D40DC (the view of
 * func_80055E40 / func_80055A68): id at +0x44 (-1 = free), run state at
 * +0x5C. */
typedef struct {
    unsigned char pad0[0x44];
    s32 id;
    unsigned char pad48[0x14];
    s16 active;
    unsigned char pad5E[2];
} Task;

extern s32 D_80139B30;
extern Task D_801D40DC[32];
void func_80055E40(void);
void func_80055FD4(void) {
    s32 i;
    if (D_80139B30 != 1) {
        return;
    }
    for (i = 0; i < 32; i++) {
        if (D_801D40DC[i].id != -1 && D_801D40DC[i].active == 2) {
            return;
        }
    }
    func_80055E40();
}

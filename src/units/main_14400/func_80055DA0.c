#include "common.h"

/* The table owns 32 complete 0x60-byte records; +0x44 is the ID and +0x5C the run state. */
typedef struct {
    unsigned char pad0[0x44];
    s32 id;
    unsigned char pad48[0x14];
    short active;
    unsigned char pad5E[2];
} Task;
extern Task D_801D40DC[32];
extern s32 D_80139B30, D_80139B34, D_80139B38;
extern void func_80055EDC(s32);
void func_80055DA0(void) { s32 i; for (i = 0; i < 32; i++) { if (D_801D40DC[i].id != -1 && D_801D40DC[i].active == 2) func_80055EDC(i); } D_80139B30 = 1; D_80139B34 = 0; D_80139B38 = 0; }

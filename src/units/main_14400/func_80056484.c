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
extern s32 D_8013A260;
extern void func_80056504(s32);
void func_80056484(void) {
    s32 index = 0;
    s32 missing = -1;
    do {
        if (D_801D40DC[index].id != missing && D_801D40DC[index].active == 0)
            func_80056504(index);
        index++;
    } while (index < 0x20);
    D_8013A260 = 0;
}

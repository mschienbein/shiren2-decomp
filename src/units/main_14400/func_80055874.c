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

extern Task D_801D40DC[32];
extern s32 D_80139B18;
extern void func_80055BD8(s32 index);
void func_80055874(void) {
    s32 index;
    for (index = 0; index < 32; index++) {
        if (D_801D40DC[index].id != -1 && D_801D40DC[index].active == 1)
            func_80055BD8(index);
    }
    D_80139B18 = 0;
}

#include "common.h"

typedef signed short s16;

/* One 0x60-byte record of the 32-entry task table at D_801D40DC (the view of
 * func_800556D4 / func_80055A68): id at +0x44 (-1 = free), dispatch index at
 * +0x50, run state at +0x5C. Only the fields reset here are named. */
typedef struct {
    unsigned char pad0[0x44];
    s32 id;
    unsigned char pad48[8];
    s16 type;
    s16 field_52;
    s16 field_54;
    s16 field_56;
    s16 field_58;
    s16 field_5A;
    s16 active;
    s16 field_5E;
} Task;

extern char D_801D8FD0[];
extern Task D_801D40DC[32];
extern void func_8006E8E0(void *),func_80055874(void),func_80056484(void),func_80055E40(void);

void func_80055400(void)
{
    s32 i;

    func_8006E8E0(D_801D8FD0);
    for (i = 0; i < 32; i++) {
        D_801D40DC[i].id = -1;
        D_801D40DC[i].type = 0;
        D_801D40DC[i].field_52 = 0;
        D_801D40DC[i].field_54 = 0;
        D_801D40DC[i].field_56 = 0;
        D_801D40DC[i].field_58 = 0;
        D_801D40DC[i].field_5A = 0;
        D_801D40DC[i].field_5E = 0;
        D_801D40DC[i].active = -1;
    }
    func_80055874();
    func_80056484();
    func_80055E40();
}

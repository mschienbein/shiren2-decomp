#include "common.h"

typedef signed short s16;

/* One 0x60-byte record of the 32-entry task table at D_801D40DC, the same
 * base and stride func_80055A68 walks: id at +0x44 (-1 = free), dispatch
 * index at +0x50, run state at +0x5C. Only the fields reset here are named. */
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

extern Task D_801D40DC[32];

void func_800556D4(s32 index)
{
    s32 none;

    D_801D40DC[index].id = none = -1;
    D_801D40DC[index].type = 0;
    D_801D40DC[index].field_52 = 0;
    D_801D40DC[index].field_54 = 0;
    D_801D40DC[index].field_56 = 0;
    D_801D40DC[index].field_58 = 0;
    D_801D40DC[index].field_5A = 0;
    D_801D40DC[index].field_5E = 0;
    D_801D40DC[index].active = none;
}

#include "common.h"
typedef struct { char pad0[4]; short state; char pad6[0x1E]; s32 v24; s32 v28; char pad2C[0x38]; s32 v64; char pad68[8]; s32 v70; } S;
extern void func_80061D28(s32, s32, s32, s32);
extern void func_8007D824(s32, s32, s32, s32);
void func_8008B4B0(S *p) {
    func_80061D28(10, 10, 0x41, 0x2B);
    func_8007D824(p->v64, p->v70, p->v24, p->v28);
    p->state = 4;
}

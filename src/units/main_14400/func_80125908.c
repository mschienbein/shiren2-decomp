#include "common.h"
typedef struct { s32 unk0, unk4; const void *unk8; } State;
extern const s32 D_80153AA0[];
extern void func_800AC68C(State *);
void func_80125908(State *arg0, s32 arg1) {
    arg1 &= 1;
    arg0->unk8 = &D_80153AA0;
    if (arg1) func_800AC68C(arg0);
}

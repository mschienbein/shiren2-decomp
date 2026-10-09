#include "common.h"
typedef struct { s32 unk0, unk4; const void *unk8; } State;
extern const unsigned char D_80160520[72];
extern State *func_801128F0(State *, s32);
State *func_80126E94(State *arg0) {
    func_801128F0(arg0, 0xE9);
    arg0->unk8 = D_80160520;
    return arg0;
}

#include "common.h"
typedef short s16;
/* List vtable slot +0x20/+0x24: s32 count(self) (func_800CE710). */
typedef struct { char unk0[0x20]; s16 unk20, unk22; s32 (*unk24)(void *); } Dispatch;
typedef struct { void *pool0; Dispatch *unk4; } State;
extern void func_800CD3D0(State *, unsigned int);
void func_800CD468(State *arg0) {
    Dispatch *dispatch = arg0->unk4;
    s32 i;
    for (i = dispatch->unk24((char *)arg0 + dispatch->unk20) - 1; i >= 0; i--) func_800CD3D0(arg0, (unsigned int)i);
}

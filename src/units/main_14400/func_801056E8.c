#include "common.h"
typedef struct VTable VTable;
typedef struct { char unk0[0x24]; VTable *unk24; } State;
extern VTable D_8015BD78;
extern void func_800EFD28(State *, s32);
extern void func_800A3918(State *);
void func_801056E8(State *arg0, s32 arg1) {
    arg0->unk24 = &D_8015BD78;
    func_800EFD28(arg0, 0);
    if (arg1 & 1) func_800A3918(arg0);
}

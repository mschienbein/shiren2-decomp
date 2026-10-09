#include "common.h"
typedef struct { char unk0[0xC0]; s32 unkC0; } State;
extern void func_800EEED8(State *);
void func_80109A24(State *arg0) { func_800EEED8(arg0); arg0->unkC0 = 0; }

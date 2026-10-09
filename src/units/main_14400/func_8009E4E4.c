#include "common.h"
typedef unsigned short u16;
typedef struct { char unk0[0x58]; char unk58[0x20]; u16 unk78; } State;
extern char *func_80048480(u16);
extern void func_800487EC(void *, s32, s32, const char *);
void func_8009E4E4(State *arg0) { func_800487EC(arg0->unk58, 0, 0, func_80048480(arg0->unk78)); }

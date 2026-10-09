#include "common.h"
typedef struct { char unk0[0xC]; s32 unkC; } State;
extern char *func_801170BC(char *, s32);
char *func_801170F4(State *arg0, char *arg1) { func_801170BC(arg1, arg0->unkC); return arg1; }

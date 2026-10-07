#include "common.h"
typedef struct { char pad0[0xE8]; s32 flagsE8; } Obj;
void func_800EE2A4(Obj *o, s32 f){o->flagsE8|=f;}

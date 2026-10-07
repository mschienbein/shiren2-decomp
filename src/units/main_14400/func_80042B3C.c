#include "common.h"
typedef struct { char pad0[0x18]; s32 unk18; } Obj;
Obj *func_800C9E10(void);
s32 func_80042B3C(void){return func_800C9E10()->unk18;}

#include "common.h"
typedef struct { char pad[0x8C]; s32 field8C; } Obj;
s32 func_800EB3A4(Obj *p) { return (p->field8C + 999) / 1000; }

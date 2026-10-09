#include "common.h"
typedef struct Obj Obj;
extern void *func_800AC5B4(s32 size, s32 alternate);
extern Obj *func_80122A20(Obj *o);
Obj *func_80122A58(void) { return func_80122A20(func_800AC5B4(0x2C, 0)); }

#include "common.h"

typedef unsigned char u8;

typedef struct { u8 pad0[0xC]; void *unkC; } Obj;
extern Obj D_80140100;
extern s32 D_80140104;
extern s32 D_80151350;
extern s32 D_80151498;
void func_80092508(void) {
    Obj *obj = &D_80140100;
    obj->unkC = &D_80151350;
    D_80140104 = 0;
    obj->unkC = &D_80151498;
}

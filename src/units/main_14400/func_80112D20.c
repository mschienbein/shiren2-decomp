#include "common.h"
typedef unsigned char u8;
extern u8 D_8015D730[];
typedef struct { u8 pad[8]; void *vt; u8 flag; } Obj;
void *func_800AC0C0(Obj *o, s32 type, s32 arg);
Obj *func_80112D20(Obj *o, s32 arg) {
    func_800AC0C0(o, 2, arg);
    o->vt = D_8015D730;
    o->flag = 0;
    return o;
}
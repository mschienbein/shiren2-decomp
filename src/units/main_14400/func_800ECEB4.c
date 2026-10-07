#include "common.h"

typedef unsigned char u8;
typedef struct { char pad[0x88]; s32 x88; char pad2[0x60]; s32 xEC; } S;
void func_800E9D20(S *p);
void *func_800E8A68(S *p, u8 kind);
s32 func_8010BEC4(void *o, u8 id);
void func_800EB35C(S *p, u8 v);
void func_800EB330(S *p, u8 v);
void func_800ECEB4(S *p) {
    void *o;
    func_800E9D20(p);
    o = func_800E8A68(p, 4);
    if (o != 0 && (u8)func_8010BEC4(o, 0x6E)) {
        func_800EB35C(p, 1);
        func_800EB330(p, 1);
    } else {
        func_800EB35C(p, 0x64);
        func_800EB330(p, 0x64);
    }
    p->xEC = p->x88;
}

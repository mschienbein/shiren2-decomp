#include "common.h"

typedef unsigned char u8;
typedef struct { u8 f0; u8 f1; char pad2[6]; void *f8; char padc; u8 fD; char pade[0x12]; u8 f20; u8 f21; } S;
extern char D_8015D518[], D_80147620[];
extern u8 D_8015699D, D_80156997, D_80156999;
void func_8010B8D0(S*, s32, s32); void func_8010EAA8(S*); s32 func_800C5844(void *rng, u8 base, u8 top);
S *func_8010EA20(S *a, s32 b){
    func_8010B8D0(a, 3, b);
    a->f8 = D_8015D518;
    func_8010EAA8(a);
    if (a->f1 == 0x41) a->fD = D_8015699D;
    a->f20 = func_800C5844(D_80147620, D_80156997, D_80156999);
    a->f21 = 0;
    return a;
}

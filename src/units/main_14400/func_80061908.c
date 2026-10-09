#include "common.h"

typedef unsigned short u16;
typedef union { s32 word; struct { u16 x, y; } p; } Pos;

extern u16 D_80169AE0, D_80169AE2, D_80169AE4, D_80169AE6, D_80169AE8, D_80169AEA;
extern u16 D_80169AEC, D_80169AEE, D_80169AF0, D_80169AF2, D_80169AF4, D_80169AF6;
/* Scroll progress: f32 (swc1 in func_80061A20, lwc1 in func_80061AB8). */
extern float D_80169AF8;
extern void func_80041798(s32, s32, s32 *, s32 *, s32 *, s32 *);
static inline s32 *word_ref(s32 *value) { return value; }

void func_80061908(Pos x, Pos y)
{
    s32 a, b, c, d;
    s32 *pa = word_ref(&a), *pb = word_ref(&b), *pc = word_ref(&c), *pd = word_ref(&d);

    func_80041798(x.word, y.word, pa, pb, pc, pd);
    D_80169AE0 = D_80169AEC;
    D_80169AE2 = D_80169AEE;
    D_80169AE4 = D_80169AF0;
    D_80169AE6 = D_80169AF2;
    D_80169AE8 = D_80169AF4;
    D_80169AEA = D_80169AF6;
    D_80169AEC = x.word;
    D_80169AEE = y.word;
    D_80169AF0 = *pa;
    D_80169AF2 = *pb;
    D_80169AF4 = *pc;
    D_80169AF6 = *pd;
    D_80169AF8 = 0.0f;
}

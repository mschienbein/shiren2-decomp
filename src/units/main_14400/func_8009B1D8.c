#include "common.h"
typedef struct { s32 a; s32 b; } Pair;
typedef struct { s32 x; s32 y; } Vec2;
typedef struct { char pad[0x34]; Pair unk34; char pad3C[0x4C]; s32 unk88; } Obj;
extern const unsigned char D_80152894[12], D_801528A0[12];
extern const s32 D_801528AC[4];
Vec2 func_8009B2B4(Obj *, Pair *);
void func_800488F0(Obj *, Vec2 *, s32, Vec2 *);
void func_8009B1D8(Obj *o, Pair *p){
    Pair cur = *p;
    Vec2 from, to;
    if (p->a == 0) {
        if (o->unk88) cur.b = D_801528AC[D_80152894[p->b]];
        else cur.b = D_801528AC[D_801528A0[p->b]];
    }
    from = func_8009B2B4(o, &cur);
    to = func_8009B2B4(o, &o->unk34);
    func_800488F0(o, &from, 0, &to);
    o->unk34 = cur;
}

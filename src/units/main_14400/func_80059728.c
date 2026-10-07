#include "common.h"
typedef struct { s32 x, y, z; } Vec3;
typedef struct { char pad[0xC]; Vec3 fC; float f18; float f1C; } SA;
extern Vec3 D_80165324;
extern float D_8016533C;
extern float D_80165340;
void func_800593F8(SA *p);
void func_80059728(SA *p) {
    func_800593F8(p);
    p->fC = D_80165324;
    p->f18 = D_8016533C;
    p->f1C = D_80165340;
}

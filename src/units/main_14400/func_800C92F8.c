#include "common.h"

typedef struct { s32 x, y; } Pos;
extern Pos D_80147664;
extern unsigned char D_80143392;
s32 func_800A251C(Pos *, Pos *);
void *func_800B1F90(Pos *);
void func_800B1CEC(s32, Pos *);
void func_800E2298(Pos *);
void func_800B2DC4(Pos *);
void func_800C92F8(Pos *s) {
    Pos p;
    Pos *pp = &p;
    Pos *last = &D_80147664;
    void *t;
    pp->x = s->x;
    pp->y = s->y;
    if (func_800A251C(pp, last)) return;
    if (D_80143392 == 0) {
        t = func_800B1F90(pp);
        if (t != 0 && t == func_800B1F90(last)) {
            func_800B1CEC(1, pp);
            return;
        }
    }
    func_800E2298(s);
    func_800B2DC4(&D_80147664);
    D_80147664 = p;
}

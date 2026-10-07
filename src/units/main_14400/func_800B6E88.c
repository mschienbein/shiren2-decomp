#include "common.h"

typedef struct { s32 x, y; } Pos;
extern s32 D_80147480;
/* The coordinate occupies 0x801C91D0..0x801C91D7; func_800A251C reads
 * its two words at offsets 0 and 4. Initialize both through the whole Pos. */
extern Pos D_801C91D0;
extern Pos *D_801476B8;
s32 func_800A251C(Pos*, Pos*); s32 func_800A5B98(Pos*, Pos*);
s32 func_80049CB4(s32 id, ...); void func_800A58FC(Pos*, Pos*);
static inline void setPosY(Pos *pos,s32 y) { pos->y=y; }
/* Base override of slot +0x24 of the D_80153B40 family: void (self). */
void func_800B6E88(void *self /* unused: the slot call contract supplies the receiver */) {
    Pos p;
    Pos t;
    s32 ok;
    if (D_80147480 == 0) { D_80147480 = 1; D_801C91D0.x = 0; setPosY(&D_801C91D0,0); }
    p = *D_801476B8;
    ok = func_800A251C(&D_801C91D0, &p) == 1;
    if (ok) {
        ok = func_800A5B98(D_801476B8, &p) == 1;
        if (!ok) {
            t.x = 10; t.y = 10;
            p = t;
        }
    }
    func_80049CB4(0x86, D_801476B8);
    func_800A58FC(D_801476B8, &p);
}

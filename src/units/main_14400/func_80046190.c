#include "common.h"
typedef struct { char pad[0x72]; unsigned char f72; } Player;
extern Player *D_801476B8;
s32 func_80049CB4(s32 id, ...);
s32 func_8006D44C(s32 a, s32 b);
s32 func_80046190(void *unused_receiver) {
    s32 i = 0;
    s32 r;
    for (;;) {
        i++;
        if (i >= 601) {
            return -1;
        }
        {
            Player *p = D_801476B8;
            s32 v = p->f72;
            s32 m = v & 0x10;
            s32 set = m != 0;
            if (set == 1) {
                p->f72 = v & ~0x10;
                func_80049CB4(0x1F, D_801476B8);
                func_80049CB4(2);
            }
        }
        r = func_8006D44C(1, 0);
        if (r != -1) {
            return r;
        }
    }
}

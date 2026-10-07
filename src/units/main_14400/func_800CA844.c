#include "common.h"
typedef struct { char pad[0x1C]; s32 f1C; unsigned char f20; } SA;
s32 func_800CA76C(SA *p, unsigned char v);
void func_800CA844(SA *p) {
    if (p->f1C == 0) {
        func_800CA76C(p, p->f20);
    }
}

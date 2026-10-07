#include "common.h"
typedef struct { unsigned char x0; unsigned char x1; s32 x4; s32 x8; } Entry;
extern Entry D_801A75F0[16];
void func_8007BDE4(void) {
    s32 i;
    Entry *e;
    for (i = 0; i < 16; i++) {
        e = &D_801A75F0[i];
        e->x0 = 0;
        e->x1 = 0;
        e->x4 = -1;
        e->x8 = -1;
    }
}

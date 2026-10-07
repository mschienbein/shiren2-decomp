#include "common.h"

typedef unsigned char u8;
typedef struct { void *f0; s32 f4; s32 f8; } S;
u8 func_800AFFD0(void *src_table, void *obj, void *dst_table);
void func_800D0FB0(S *s, void *arg1, void **arg2) {
    func_800AFFD0(*arg2, arg1, s->f0);
    s->f8 = 1;
}

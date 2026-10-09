#include "common.h"
typedef struct { char pad[0x4C]; const void *f4C; } SA;
extern const unsigned char D_801521D0[144];
SA *func_800953C0(SA *p);
void func_80097240(void *n, void *record, void *pairs, void *counts);
SA *func_800975A8(SA *p, void *record, void *pairs, void *counts) {
    func_800953C0(p);
    p->f4C = D_801521D0;
    func_80097240(p, record, pairs, counts);
    return p;
}

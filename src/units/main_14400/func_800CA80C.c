#include "common.h"

typedef struct { char pad[0x1C]; void *f1C; } S;
void func_800CA2C0(void *);
void func_800CA80C(S *s) {
    if (s->f1C != 0) {
        func_800CA2C0(s->f1C);
    }
    s->f1C = 0;
}

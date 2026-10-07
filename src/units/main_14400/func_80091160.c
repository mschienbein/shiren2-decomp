#include "common.h"

typedef struct { char pad[0x20]; void *f20; } S;
void func_80091544(void *);
void func_80091160(S *s) {
    if (s->f20 != 0) {
        func_80091544(s->f20);
        s->f20 = 0;
    }
}

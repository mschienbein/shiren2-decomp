#include "common.h"

typedef struct { char pad0[0xC]; void *field_C; } S;
void func_80091544(void *p);
void func_8008DB10(S *s) {
    if (s->field_C != 0) {
        func_80091544(s->field_C);
        s->field_C = 0;
    }
}

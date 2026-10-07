#include "common.h"
typedef struct { unsigned char x0; } S;
typedef struct { char pad[8]; unsigned char x8; } T;
void *func_800A6538(void *out_direction, void *obj, void *target);
void *func_800A65E4(S *p, T *q, void *target) {
    if (target == 0) {
        p->x0 = q->x8;
    } else {
        func_800A6538(p, q, target);
    }
    return p;
}

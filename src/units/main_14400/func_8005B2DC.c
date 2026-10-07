#include "common.h"
typedef struct { float v[11]; } M;
s32 func_8005B2DC(M *a, M *b) {
    return a->v[0] == b->v[0] && a->v[1] == b->v[1] && a->v[2] == b->v[2] &&
           a->v[3] == b->v[3] && a->v[4] == b->v[4] && a->v[5] == b->v[5] &&
           a->v[6] == b->v[6] && a->v[7] == b->v[7] && a->v[8] == b->v[8] &&
           a->v[9] == b->v[9] && a->v[10] == b->v[10];
}

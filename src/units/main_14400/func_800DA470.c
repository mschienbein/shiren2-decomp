#include "common.h"
typedef struct { unsigned short f0; char pad[2]; void *f4; } SD;
extern char D_80157FA8[];
extern char D_801582D8[];
SD *func_800DA470(SD *p) {
    p->f4 = D_80157FA8;
    p->f0 = 0x3A;
    p->f4 = D_801582D8;
    return p;
}

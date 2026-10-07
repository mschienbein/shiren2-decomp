#include "common.h"

extern unsigned char D_80145460[];
extern char D_80143094[];
void *func_800AFD78(void *table, unsigned char index);
typedef struct { s32 x; s32 y; } Pos;
void *func_800B4D80(Pos *p) {
    s32 bad = 0;
    unsigned char v;
    if (p->y >= 0x4C || p->x >= 0x36 || p->y < 0 || p->x < 0) bad = 1;
    if (bad) return 0;
    v = D_80145460[p->y + p->x * 0x4C];
    if (v == 0xFF) return 0;
    return func_800AFD78(D_80143094, v);
}

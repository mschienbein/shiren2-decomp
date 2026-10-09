#include "common.h"

typedef struct { unsigned char id; unsigned char weight; } Pick;
extern char D_80147620[];
extern unsigned short func_800C58DC(void *, unsigned short);
/* Callers supply the mode ABI slot; this chooser does not use it. */
s32 func_800AB2B0(Pick *table, s32 mode) {
    Pick *p;
    unsigned short total;
    unsigned short roll;
    if (table == 0) return 0;
    for (p = table, total = 0; p->id != 0; p++) total += p->weight;
    if (total == 0) return 0;
    roll = func_800C58DC(D_80147620, total - 1);
    for (; table->id != 0; table++) {
        if (roll < table->weight) return table->id;
        roll -= table->weight;
    }
    return 0;
}

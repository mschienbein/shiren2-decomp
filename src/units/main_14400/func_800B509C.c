#include "common.h"
typedef struct { s32 field_0; s32 field_4; } Pair;
extern unsigned char D_80143390;
extern Pair D_80143368[];
void func_800B509C(void)
{
    s32 i;
    Pair *p;
    D_80143390 = 0;
    i = 0;
    p = D_80143368;
    for (;;) {
        s32 more = i < 4;
        if (!more) break;
        p->field_0 = 0;
        p->field_4 = 0;
        i++;
        p++;
    }
}

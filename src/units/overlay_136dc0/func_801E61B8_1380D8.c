#include "func_801E61B8_1380D8/types.h"

void func_801E61B8_1380D8(s32 index, s32 resource, s32 value, s32 duration)
{
    OvlRecord *p = &D_801F6764_148684[index];
    s16 i;
    if (!resource) {
        func_801E6140_138060(index);
    } else {
        for (i = 0; i < 8; i++) {
            if (!p->h2E[i]) {
                p->h2E[i] = resource;
                p->b19[i] = duration;
                p->b21[i] = value;
                if (i == 0) p->b05 = 0;
                if (i + 1 < 8) p->h2E[i + 1] = 0;
                break;
            }
        }
    }
}

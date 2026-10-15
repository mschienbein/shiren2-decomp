#include "func_801E6264_138184/types.h"

void func_801E6264_138184(void)
{
    s32 i = 30;
    while (--i != -1) {
        OvlRecord *p = &D_801F6764_148684[i];
        if (p->h2E[0] && (p->h2E[1] || !p->h2A)) {
            s16 elapsed = 1;
            s16 selected = -1;
            s32 j;
            p->h2A++;
            for (j = 0; j < 8; j++) {
                if (!p->h2E[j]) {
                    p->h2A = 1;
                    selected = 0;
                    break;
                }
                if (elapsed == p->h2A) {
                    selected = j;
                    break;
                }
                elapsed += p->b19[j];
                if (p->h2A < elapsed) break;
            }
            if (selected == -1) continue;
            p->b05 = 1;
            func_80076044(i, p->h2E[selected], 2, p->b21[selected], 0);
        }
    }
}

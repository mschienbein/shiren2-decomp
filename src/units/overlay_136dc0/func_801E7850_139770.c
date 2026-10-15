#include "func_801E7850_139770/types.h"

extern Rec801F6764 D_801F6764_148684[30];

void func_801E7850_139770(void) {
    Rec801F6764 *rec = &D_801F6764_148684[30];
    s32 i;

    for (i = 29; i != -1; i--) {
        rec--;
        rec->xA = 0;
    }
}

#include "func_801E7414_139334/types.h"

extern Rec801F6764 D_801F6764_148684[30];
extern void func_8007935C(s32 i);

void func_801E7414_139334(void) {
    Rec801F6764 *rec = &D_801F6764_148684[30];
    s32 i;

    for (i = 29; i != -1; i--) {
        rec--;
        if (rec->x14 != 0) {
            func_8007935C(rec->x14 - 1);
            rec->x14 = 0;
        }
    }
}

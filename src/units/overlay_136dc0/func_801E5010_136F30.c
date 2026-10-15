#include "func_801E5010_136F30/types.h"

char *func_80083C90(char *dst, char *src);

/* Returns 1 (status). ROM leaves v0 = 1 at exit (801E50FC li v0,1), and the final
 * x9F1 increment reuses that register (addu v1,v1,v0). The only caller,
 * func_801EC568_13E488 at 801EC5D4, discards v0, so its declaration must become
 * s32 func_801E5010_136F30(void) (contract change reported to the integrator). */
s32 func_801E5010_136F30(void) {
    D_801F51E8_147108->x99B = 0;
    D_801F51E8_147108->x8F0[D_801F51E8_147108->x9F1] = 0;
    D_801F51E8_147108->x126[D_801F51E8_147108->x11E] = 0;
    if (D_801F51E8_147108->x99D == 1) {
        D_801F51E8_147108->x99D = 0;
        D_801F51E8_147108->x8F0[D_801F51E8_147108->x9F1] += 1;
    }
    func_80083C90(D_801F51E8_147108->x2B0[D_801F51E8_147108->x9F1], D_801F51E8_147108->x126);
    if (D_801F51E8_147108->x998 == 1 || D_801F51E8_147108->x999 != 2) {
        D_801F51E8_147108->x8F0[D_801F51E8_147108->x9F1] += 2;
    }
    D_801F51E8_147108->x998 = 0;
    D_801F51E8_147108->x11E = 0;
    D_801F51E8_147108->x9F1++;
    return 1;
}

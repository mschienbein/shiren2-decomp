#include "common.h"

extern unsigned short D_8015691C;
extern unsigned short D_8015698C;
extern unsigned short D_8015698E;

s32 func_800E1CC4(void *obj, s32 kind);

/* Entity slot +0x54 override s32 (void *self, s32 mode), bound at D_801496E0/D_80149778/D_80149810+0x54. */
s32 func_800E22E8(void *self, s32 mode) {
    switch (mode) {
    /* ODD_C: modes 0, 2 and 3 have no override value and fall to return 0; the labels
     * shapes codegen: without it 25 words differ (108 vs 124 bytes). */
    case 0:
    case 2:
    case 3:
        break;
    case 1:
        return 100 - D_8015691C;
    case 4:
        if (func_800E1CC4(self, 5) != 0) {
            return D_8015698E;
        }
        return D_8015698C;
    }
    return 0;
}

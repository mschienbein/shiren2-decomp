#include "common.h"

typedef struct {
    s32 field0;
    s32 count4;
} Rec_8012CE84;

/* Null-terminated list of record pointers. */
extern Rec_8012CE84 *D_80148A84[];

s32 func_8012CE84(void) {
    s32 i;
    s32 max = 0;

    for (i = 0; D_80148A84[i] != 0; i++) {
        if (max < D_80148A84[i]->count4) {
            max = D_80148A84[i]->count4;
        }
    }
    return max;
}

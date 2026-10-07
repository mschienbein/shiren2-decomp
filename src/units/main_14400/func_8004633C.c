#include "common.h"

typedef unsigned char u8;
/* func_80046270 clears this complete 0x48-byte object; the save/load
 * virtuals in func_80046294/func_800462E8 transfer the same extent.
 * D_80138C18 is an interior label at values, not a separate object. */
typedef struct {
    u8 color[4];
    u8 state[0x20];
    float values[8];
    u8 active;
    u8 pad45[3];
} SavedState;
extern s32 D_80138BF0;
extern SavedState D_80138BF4;
void func_8005C630(void *a, void *b, void *c, void *d);
void func_8004633C(void) {
    if (D_80138BF0 != 0) {
        func_8005C630(D_80138BF4.values, &D_80138BF4.active,
                     D_80138BF4.state, D_80138BF4.color);
    }
}

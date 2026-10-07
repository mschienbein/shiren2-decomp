#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
extern u16 D_801569E8;
s32 func_800E1CC4(void *arg0, s32 flag);
/* self: the delta-adjusted object pointer (unused by the helper); source: the payload source
 * object, stored by func_80136910 into its void *field_0. */
void func_80119670(void *self, void *source, void *arg2, short amount);

/* Method at D_8015E360+0x4C; callers pass the delta-adjusted object pointer in a0. */
void func_80119830(void *self, void *source, void *arg2) {
    s32 scale;

    if (func_800E1CC4(arg2, 3)) {
        scale = 2;
    } else {
        scale = 1;
    }
    func_80119670(self, source, arg2, D_801569E8 * scale);
}

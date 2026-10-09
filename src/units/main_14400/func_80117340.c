#include "common.h"

typedef unsigned short u16;
typedef short s16;
extern const u16 D_801569D6;
extern const u16 D_801569D8;
extern s32 func_800E1CC4(void *object, s32 kind);
extern s16 func_800E0690(void *object, s16 first, s16 second);
extern s32 func_80049CB4(s32 id, ...);

/* self is unused but retained as the original virtual action receiver. */
void func_80117340(void *self, void *object)
{
    s16 scale = func_800E1CC4(object, 3) ? 2 : 1;
    if (func_800E0690(object, D_801569D6 * scale, D_801569D8 * scale) > 0)
        func_80049CB4(0x128, 0x6C);
}

#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
extern u16 D_80157358[], D_80157180[], D_80157248[], D_801575E0[], D_80157728[], D_80157648[], D_801572B8[];
extern u8 D_80157448[];
s32 func_8010C174(void *obj, u8 kind, u16 *base_values, u8 *modifier_scales, u16 *values_1, u16 *values_2, u16 *values_3, u16 *values_4, u16 *values_5, u16 *values_6);
s32 func_8010EE80(u8 *a){ return func_8010C174(a, a[1], D_801572B8, D_80157448, D_80157358, D_80157180, D_80157248, D_801575E0, D_80157728, D_80157648); }

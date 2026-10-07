#include "common.h"
typedef struct { short field_0; short field_2; void *volatile field_4; } Object;
extern s32 D_80157FA8[], D_80158218[];
Object *func_800DA268(Object *arg) {
    arg->field_4 = D_80157FA8;
    arg->field_0 = 0x3B;
    arg->field_4 = D_80158218;
    return arg;
}

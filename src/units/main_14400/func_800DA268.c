#include "common.h"
typedef struct { short field_0; short field_2; void *field_4; } Object;
extern s32 D_80157FA8[], D_80158218[];
/* The caller supplies a payload, but this fixed-kind constructor does not read it. */
Object *func_800DA268(Object *arg, unsigned char *unused_payload) {
    /* Base-command initialization (func_800D906C), then the derived vtable. */
    arg->field_4 = D_80157FA8;
    arg->field_0 = 0x3B;
    arg->field_4 = D_80158218;
    return arg;
}

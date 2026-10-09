#include "common.h"
typedef struct { unsigned short field_0; void *field_4; } Object;
extern unsigned char D_80157FA8[], D_80158278[];
Object *func_800DA350(Object *arg) {
    /* Base-command initialization (func_800D906C), then the derived vtable. */
    arg->field_4 = D_80157FA8;
    arg->field_0 = 0x38;
    arg->field_4 = D_80158278;
    return arg;
}

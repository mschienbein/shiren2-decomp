#include "common.h"
typedef struct { unsigned short field_0; void * volatile field_4; } Object;
extern unsigned char D_80157FA8[], D_80158278[];
Object *func_800DA350(Object *arg) { arg->field_4 = D_80157FA8; arg->field_0 = 0x38; arg->field_4 = D_80158278; return arg; }

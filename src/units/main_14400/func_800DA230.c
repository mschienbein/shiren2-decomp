#include "common.h"
extern s32 D_80157FA8[], D_80158218[];
typedef struct { unsigned short field_0; s32 *field_4; } Obj;
Obj *func_800DA230(Obj *a) { a->field_4 = D_80157FA8; a->field_0 = 0x3B; a->field_4 = D_80158218; return a; }

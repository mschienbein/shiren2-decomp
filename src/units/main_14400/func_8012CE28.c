#include "common.h"
extern s32 D_801488F8[];
typedef struct { char pad[0x1C]; unsigned char field_1C; s32 *field_20; } Obj;
void func_8012CE28(Obj *a) { a->field_1C = 6; a->field_20 = D_801488F8; }

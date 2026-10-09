#include "common.h"
typedef struct { unsigned char field_00[0x50]; unsigned short field_50; } Object;
extern unsigned short D_80158C6C[];
extern s32 func_800E04D0(Object *);
void func_800E4DE8(Object *self) { self->field_50 = D_80158C6C[func_800E04D0(self) & 0xff]; }

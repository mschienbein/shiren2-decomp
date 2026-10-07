#include "common.h"
typedef struct { unsigned char field_0[0x38]; short field_38; void (*field_3C)(void *, s32 *); } VTable;
typedef struct { s32 field_0[2]; VTable *field_8; } Object;
void func_800AE610(Object *arg, s32 value) { s32 message[8]; message[0] = 0x10; message[1] = value; arg->field_8->field_3C((unsigned char *)arg + arg->field_8->field_38, message); }

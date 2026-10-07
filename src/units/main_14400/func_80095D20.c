#include "common.h"
typedef struct { unsigned char field_0[0x18]; short field_18; void (*field_1C)(void *); } VTable;
typedef struct { unsigned char field_0[0x46]; unsigned char field_46; unsigned char field_47[5]; VTable *field_4C; } Object;
extern void func_80048728(void *);
void func_80095D20(Object *arg) { arg->field_46 = 1; arg->field_4C->field_1C((unsigned char *)arg + arg->field_4C->field_18); func_80048728(arg); }

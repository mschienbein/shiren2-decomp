#include "common.h"
typedef struct { unsigned char pad[0x2C]; unsigned short field2C; } Object;
void func_800E0CF4(Object *p, unsigned short percent) { p->field2C = p->field2C * percent / 100; }

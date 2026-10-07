#include "common.h"
typedef struct { unsigned char field_0[0x18]; short field_18; short field_1A; void (*field_1C)(void *,s32,void *); } VTable;
typedef struct { unsigned char field_0[0x18]; VTable *field_18; } Object;
extern unsigned char D_8015D694[];
extern void func_800AF11C(void *,Object *);
extern void func_800CA4A4(Object *,void *);
void func_80112BE8(unsigned char *arg,Object *other) { func_800AF11C(arg,other); func_800CA4A4(other,D_8015D694); other->field_18->field_1C((char *)other+other->field_18->field_18,4,arg+0xC); }

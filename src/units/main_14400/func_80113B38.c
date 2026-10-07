#include "common.h"
typedef struct { unsigned char field_0[0x18]; short field_18; void (*field_1C)(void *, s32, void *); } VTable;
typedef struct { unsigned char field_0[0x18]; VTable *field_18; } Object;
extern unsigned char D_8015D87C[];
extern void func_800AF11C(void *, Object *);
extern void func_800CA4A4(Object *, void *);
void func_80113B38(unsigned char *arg, Object *object) { func_800AF11C(arg, object); func_800CA4A4(object, D_8015D87C); object->field_18->field_1C((unsigned char *)object + object->field_18->field_18, 2, arg + 12); }

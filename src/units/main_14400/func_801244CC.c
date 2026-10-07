#include "common.h"

typedef struct { char pad[0x28]; short offset28; short pad2A; void (*method2C)(void *,s32,void *); } VTable;
typedef struct { char pad[0x18]; VTable *vtable; } Target;
extern char D_8015FE74[];
extern void func_8011640C(void *,Target *),func_800CA4E8(Target *,void *);
void func_801244CC(char *obj,Target *target) { func_8011640C(obj,target); func_800CA4E8(target,D_8015FE74); target->vtable->method2C((char *)target+target->vtable->offset28,1,obj+0x10); }

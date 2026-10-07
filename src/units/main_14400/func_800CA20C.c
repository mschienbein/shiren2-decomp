#include "common.h"

typedef struct { char pad[8]; short offset8; short padA; void (*methodC)(void *); } VTable;
typedef struct { char pad[0xC]; s32 fieldC; char pad10[8]; VTable *vtable; } Object;
void func_800CA20C(Object *obj,s32 value) { obj->vtable->methodC((char *)obj+obj->vtable->offset8); obj->fieldC=value; }

#include "common.h"

typedef struct Reader Reader;
typedef struct { char pad[2]; unsigned char count; char pad3[5]; void **items; } Object;
extern void *func_8008F76C(Reader *,s32);
s32 func_8008FA28(Reader *a,s32 b,Object *obj) { s32 result=0; void *item=func_8008F76C(a,b); if (!item) result=-1; else obj->items[obj->count++]=item; return result; }

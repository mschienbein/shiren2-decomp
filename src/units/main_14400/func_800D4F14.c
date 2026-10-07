#include "common.h"

/* Pool container: pool-record pointer at +0 (cleared by func_8013687C), vtable at +4. */
typedef struct { void *pool; void *field4; } Object;
extern char D_80149DB8[],D_80154848[];
extern void func_8013687C(void **pool);
extern void func_800D4F04(Object *);
Object *func_800D4F14(Object *obj) { obj->field4=D_80149DB8; func_8013687C(&obj->pool); obj->field4=D_80154848; func_800D4F04(obj); return obj; }

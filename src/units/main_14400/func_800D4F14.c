#include "common.h"
#include "item_records.h"

/* Storage object: a pool handle (derived vtable D_80154848); only the handle is accessed. */
typedef struct { Handle base; } Object;
extern void func_800D4F04(Object *);
Object *func_800D4F14(Object *obj) { obj->base.vtable=&D_80149DB8; func_8013687C(&obj->base.pool); obj->base.vtable=&D_80154848; func_800D4F04(obj); return obj; }

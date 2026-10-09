#include "common.h"

/* List vtable slot +0x10/+0x14: s32 query(self) (e.g. func_800CEA78 returns byte +0xD). */
typedef struct { unsigned char unknown00[0x10]; short adjust10; short unknown12; s32 (*method14)(void *); } VTable;
typedef struct { void *pool00; VTable *field04; } Object;
extern s32 func_800CD1FC(Object *);
s32 func_800CD278(Object *object) {
    VTable *table = object->field04;
    s32 first = table->method14((char *)object + table->adjust10);
    return first - func_800CD1FC(object);
}

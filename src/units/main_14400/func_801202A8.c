#include "common.h"

typedef struct Object_80120270 Object_80120270;

void *func_800AC5B4(s32 size, s32 alternate);
Object_80120270 *func_80120270(Object_80120270 *obj);

/* Factory slot of table D_80157964: allocate 0xC bytes and construct. */
Object_80120270 *func_801202A8(void) {
    return func_80120270(func_800AC5B4(0xC, 0));
}

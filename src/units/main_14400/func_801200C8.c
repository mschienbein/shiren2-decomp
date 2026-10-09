#include "common.h"
typedef struct Object Object;
extern void *func_800AC5B4(s32 size, s32 alternate);
extern Object *func_80120090(Object *obj);
Object *func_801200C8(void) { return func_80120090(func_800AC5B4(12, 0)); }

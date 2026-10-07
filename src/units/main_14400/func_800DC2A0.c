#include "common.h"
typedef struct { s32 field_00; void *field_04; } Object;
extern char D_801585C8[];
extern void *func_800DA8A0(void *obj, s32 kind, void *src);
Object *func_800DC2A0(Object *object, void *src) { func_800DA8A0(object, 0x17, src); object->field_04 = D_801585C8; return object; }

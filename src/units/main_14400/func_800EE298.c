#include "common.h"
/* +0x104 retains an object pointer (func_800EE274 compares it with a pointer argument,
 * func_800EE290 returns it). */
typedef struct { unsigned char pad[0x104]; void *field_104; } Object;
s32 func_800EE298(Object *obj) { return obj->field_104 != 0; }

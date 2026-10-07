#include "common.h"

typedef struct { char pad[0x4C]; void *field4C; } Object;
extern char D_80151E38[];
extern void func_800D8FA8(void *object);
void func_8009F4BC(Object *obj,s32 flags) { obj->field4C=D_80151E38; if (flags & 1) func_800D8FA8(obj); }

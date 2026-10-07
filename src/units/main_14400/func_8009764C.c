#include "common.h"
typedef struct { unsigned char field_00[0x4C]; void *field_4C; } Object;
extern char D_80151E38[];
extern void func_800D8FA8(void *object);
void func_8009764C(Object *object, s32 flags) { object->field_4C = D_80151E38; if (flags & 1) func_800D8FA8(object); }

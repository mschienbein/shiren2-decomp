#include "common.h"
extern s32 D_80151E38[];
typedef struct { char pad[0x4C]; s32 *field_4C; } Obj;
extern void func_800D8FA8(void *object);
void func_8009713C(Obj *a, s32 b) { a->field_4C = D_80151E38; if (b & 1) func_800D8FA8(a); }

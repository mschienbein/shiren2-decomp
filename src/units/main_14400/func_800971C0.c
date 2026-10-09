#include "common.h"
typedef struct { unsigned char field_0[0x4C]; const void *field_4C; } Object;
extern const unsigned char D_80151E38[144];
extern void func_800D8FA8(void *object);
void func_800971C0(Object *arg, s32 flags) { arg->field_4C = D_80151E38; if (flags & 1) func_800D8FA8(arg); }

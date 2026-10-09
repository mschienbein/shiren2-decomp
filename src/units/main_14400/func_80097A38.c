#include "common.h"
typedef struct { char pad[0x4C]; const void *field_4c; } Obj;
extern const unsigned char D_80151E38[144];
extern void func_800D8FA8(void *object);
void func_80097A38(Obj *p,s32 flags) { p->field_4c=D_80151E38; if(flags & 1) func_800D8FA8(p); }

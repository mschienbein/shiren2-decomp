#include "common.h"
typedef unsigned char u8;
typedef struct Obj800A38A0 Obj800A38A0;
void func_800F479C(u8 *self, s32 flags);
void func_800A3918(Obj800A38A0 *obj);
void func_800F5858(void *obj, s32 flags) { func_800F479C(obj, 0); if (flags & 1) func_800A3918(obj); }

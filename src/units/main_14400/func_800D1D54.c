#include "common.h"

typedef struct { char pad[0xBB]; signed char slots[12]; } Object;
void func_800D1D54(Object *obj) { s32 i=12; for (;;) { s32 index=--i; if (index == -1) break; obj->slots[index]=-1; } }

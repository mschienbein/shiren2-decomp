#include "common.h"

typedef struct { s32 field0; s32 field4; s32 field8; void *fieldC; } Object;
extern Object D_801400F0;
extern s32 D_801400F4;
extern char D_80151350[], D_80149F30[];
void func_800923BC(void) { Object *obj=&D_801400F0; obj->fieldC=D_80151350; D_801400F4=0; obj->fieldC=D_80149F30; }

#include "common.h"

typedef struct { char pad[0x5C]; s32 field5C; s32 field60; } Object;
void func_800E2568(Object *obj) { obj->field5C=0; obj->field60=0; }

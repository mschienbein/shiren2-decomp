#include "common.h"
typedef struct { s32 field_0; void *field_4; } Object;
extern s32 D_80158538[];
extern void *func_800DA904(void *obj, s32 kind, unsigned char *params);
Object *func_800DBC88(Object *arg,unsigned char *value) { func_800DA904(arg,0x14,value); arg->field_4=D_80158538; return arg; }

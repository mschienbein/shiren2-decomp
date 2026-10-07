#include "common.h"
typedef unsigned char u8;

typedef struct { s32 field0; void *field4; } Object;
extern char D_801587A8[];
extern void *func_800DA904(Object *, s32, u8 *);
Object *func_800DD378(Object *obj, u8 *params) { func_800DA904(obj,0x21,params); obj->field4=D_801587A8; return obj; }

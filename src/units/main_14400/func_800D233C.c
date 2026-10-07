#include "common.h"
typedef struct { s32 field_0; void *field_4; } Object;
extern s32 func_800B68B0(void *);
extern void *func_800B6A98(void *out, void *room, s32 index);
extern void func_800D2260(Object *,s32 *);
void func_800D233C(Object *arg) { s32 item[2]; s32 i=0; s32 count=func_800B68B0(arg->field_4); for(;i<count;++i) { func_800B6A98(item,arg->field_4,i); func_800D2260(arg,item); } }

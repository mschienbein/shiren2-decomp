#include "common.h"
typedef unsigned char u8;
typedef struct List List;
typedef struct { List *list_0; void *vtable_4; s32 field_8; } Owner;
extern u32 func_800D07D8(Owner *owner, s32 index);
extern void func_800AFCA8(void *table, u8 id);
void func_800D0BE0(Owner *p, s32 index) { u32 id=func_800D07D8(p, index); func_800AFCA8(p->list_0, id & 0xFF); p->field_8=1; }

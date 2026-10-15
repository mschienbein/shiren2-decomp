#include "common.h"
#include "item_records.h"
/* Item container: pool +0, method table +4, modified flag +8. */
typedef struct { ItemPool *list_0; void *vtable_4; s32 field_8; } Owner;
void func_800D0BE0(Owner *p, s32 index) { u32 id=func_800D07D8(&p->list_0, index); func_800AFCA8(p->list_0, id & 0xFF); p->field_8=1; }

#include "common.h"
typedef unsigned char u8;
typedef struct { u8 pad_0[0x98]; short delta_98, index_9A; u8 *(*list_9C)(u8 *); } VTable;
typedef struct { u8 pad_0[0x24]; VTable *vtable_24; } Object;
typedef struct { void *list_0; s32 field_4, field_8; } Owner;
extern Object *D_801476B8;
/* Slot 0x9C includes func_800EE07C. */
void func_800A0DA8(void *arg, void *list) { Owner *p=arg; if (list==0) { VTable *v=D_801476B8->vtable_24; p->list_0=v->list_9C((u8 *)D_801476B8+v->delta_98); } else p->list_0=list; p->field_4=0; p->field_8=-1; }

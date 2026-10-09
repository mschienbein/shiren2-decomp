#include "common.h"
typedef unsigned char u8;
typedef struct { u8 pad_0[0x60]; short delta_60; short index_62; void (*reset_64)(void *); } VTable;
typedef struct { u8 pad_0[0x24]; VTable *vtable_24; u8 pad_28[0x58]; s32 field_80, field_84, field_88, field_8C, field_90, field_94, field_98; u8 field_9C; u8 pad_9D[3]; s32 field_A0, field_A4; } Obj800F5F60;
/* Slot 0x64 in D_801599F8 is func_800F62F0. */
void func_800F5FB4(Obj800F5F60 *p) { VTable *v=p->vtable_24; p->field_80=0; p->field_84=0; p->field_88=0; p->field_8C=0; p->field_90=0; p->field_94=0; p->field_98=0; p->field_9C=0; p->field_A0=0; p->field_A4=0; v->reset_64((char *)p+v->delta_60); }

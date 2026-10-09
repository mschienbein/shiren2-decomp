#include "common.h"
typedef unsigned char u8;
typedef struct { u8 pad_0[8]; const void *vtable_8; u8 field_C, field_D, field_E, field_F, field_10; } Object;
extern void *func_80117230(void *obj, s32 kind);
extern const u8 D_80160720[];
void *func_80128280(void *arg) { Object *p=arg; func_80117230(p,0xF1); p->vtable_8=D_80160720; p->field_C=0; p->field_D=0; p->field_E=0; p->field_F=0; p->field_10=1; return p; }

#include "common.h"
typedef struct { char pad[8]; short field_8; void (*field_C)(void *, s32); } VTable;
typedef struct { unsigned char field_0; char pad[7]; VTable *field_8; } Obj;
extern char D_80142DE0[];
extern void *func_800AB16C(void *table, unsigned char pick, s32 mode);
extern void *func_800AC244(unsigned char id);
Obj *func_800AAC48(s32 a) { s32 n = 100; for (;;) { s32 old = n--; if (old <= 0) break; { Obj *p = func_800AB16C(D_80142DE0, 0, a); if (!p || p->field_0 != 10) return p; p->field_8->field_C((char *)p + p->field_8->field_8, 3); } } return func_800AC244(0x16); }

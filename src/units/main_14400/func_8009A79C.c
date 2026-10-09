#include "common.h"
/* Twelve-byte menu choice record: command, submenu pointer (stored at 0x8009966C), value. */
typedef struct { s32 field0; void *submenu4; s32 field8; } Entry;
typedef struct { char pad0[0x20]; s32 field20; char pad24[0x10]; s32 field34; s32 field38; char pad3C[0x14]; Entry *field50; } Obj;
extern void func_8009A66C(Obj *, s32);
void func_8009A79C(Obj *p) { if (p->field50[p->field34 + p->field20 * p->field38].field8 == 15) func_8009A66C(p, 1); else func_8009A66C(p, 0); }

#include "common.h"
typedef short s16;
typedef unsigned char u8;
typedef struct { char pad[0x18]; s16 offset18; s16 field1A; s32 (*call1C)(void *, s32); } Table;
typedef struct { u8 field0; char pad1[7]; Table *field8; } Obj;
extern char *func_800AC990(Obj *);
extern char *func_800AE674(Obj *);
extern char *func_80114BCC(Obj *, char *, s32);
char *func_800AC9D8(Obj *p) { Table *t = p->field8; if (t->call1C((char *)p + t->offset18, 30)) { return func_800AC990(p); } else { char *value = func_800AE674(p); if (p->field0 == 9) func_80114BCC(p, value, 0); return value; } }

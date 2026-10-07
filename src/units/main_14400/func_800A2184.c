#include "common.h"
typedef struct { s32 field_0, field_4; } Obj;
extern char *func_801170BC(char *destination, s32 textId);
char *func_800A2184(Obj *p, char *destination) { return func_801170BC(destination, p->field_4); }

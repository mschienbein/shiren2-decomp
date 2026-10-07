#include "common.h"
typedef struct { char pad[0xC]; unsigned char field_c; } Obj;
void func_80128C24(Obj *p, s32 v) { p->field_c=v; }

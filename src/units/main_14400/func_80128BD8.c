#include "common.h"
typedef struct { char pad[0xE]; unsigned char field_E; } Obj;
void func_80128BD8(Obj *a, s32 b) { a->field_E = b; }

#include "common.h"
typedef struct { unsigned char pad0[0x60]; unsigned char field_60[0x10]; } Obj;
extern void func_80048728(void *);
void func_8009AD08(Obj *obj) { func_80048728(obj->field_60); }

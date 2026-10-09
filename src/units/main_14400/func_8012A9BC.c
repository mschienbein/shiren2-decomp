#include "common.h"

typedef unsigned char u8;

/* Bank +0x10 is the channel-record pointer consumed by func_8012A158 and func_8012A1DC. */
typedef struct { u8 pad0[0x10]; void *unk10; } Obj;
void func_8012A9BC(Obj *obj, void *value) { obj->unk10 = value; }

#include "common.h"
typedef unsigned char u8;
typedef struct Obj Obj;
void *func_800A38FC(s32 size);
Obj *func_800F8EB0(Obj *self, u8 kind);
Obj *func_800F8E70(u8 kind, Obj *storage) { Obj *result; if (!storage) result = func_800F8EB0(func_800A38FC(0xA0), kind); else result = func_800F8EB0(storage, kind); return result; }

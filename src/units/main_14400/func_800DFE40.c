#include "common.h"
typedef struct { unsigned short field0; char pad2[2]; void *field4; } Obj;
extern unsigned char D_80157FA8[];
extern unsigned char D_80158B98[];
Obj *func_800DFE40(Obj *obj) { obj->field4 = D_80157FA8; obj->field0 = 0x3D; obj->field4 = D_80158B98; return obj; }

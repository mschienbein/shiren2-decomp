#include "common.h"
typedef unsigned char u8;
typedef struct { char pad0[0x24]; void *field24; } Obj800EFC70;
Obj800EFC70 *func_800EFC70(Obj800EFC70 *obj, s32 arg1, u8 arg2);
extern unsigned char D_8015A130[];
Obj800EFC70 *func_800FB4F8(Obj800EFC70 *obj, u8 kind) { func_800EFC70(obj, 0x21, kind); obj->field24 = D_8015A130; return obj; }

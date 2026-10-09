#include "common.h"
typedef unsigned char u8;
typedef struct { u8 pad_0[0x21]; u8 field_21; } S;
extern s32 func_8010BEC4(S *, u8);
extern unsigned short D_80156990;
s32 func_8010EBA4(void *arg) { S *object = arg; s32 result = 0; if ((u8)func_8010BEC4(object, 0x4A)) result = (u32)object->field_21 >= D_80156990; return result; }

#include "common.h"
typedef unsigned char u8;
typedef struct { u8 pad_0[0x90]; short offset_90; short pad_92; s32 (*method_94)(void *, s32, s32, u8, s32); } VTable;
typedef struct { u8 pad_0[0x24]; VTable *field_24; } Obj;
s32 func_800E2B48(Obj *obj) { return obj->field_24->method_94((u8 *)obj + obj->field_24->offset_90, 1, 12, 0, 0); }

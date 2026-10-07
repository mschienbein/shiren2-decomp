#include "common.h"
typedef unsigned char u8;
typedef unsigned short u16;
typedef struct { u8 pad0[4]; u16 state; u8 pad6[0xE]; s32 handle; } Obj;
void func_80042758(s32 h);
void func_80086C54(Obj *o) { func_80042758(o->handle); o->state = 4; }

#include "common.h"
typedef unsigned char u8;
typedef struct { u8 pad[2]; u8 field2; } Obj;
typedef struct { s32 x, y; } Position;
extern Obj *func_800B4E18(Position *);
extern s32 func_80049CB4(s32, ...);
void func_800AD868(Position *position) { Obj *p = func_800B4E18(position); if (p) { p->field2 &= ~0x20; func_80049CB4(215, position); } }

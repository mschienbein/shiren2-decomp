#include "common.h"
typedef unsigned char u8;
typedef struct { char pad[0x1E]; u8 field1E; } Obj;
extern s32 func_800A8974(Obj *);
s32 func_800A3934(Obj *p) { s32 flag = (p->field1E >> 2) & 1; if (flag) return 1; return func_800A8974(p); }

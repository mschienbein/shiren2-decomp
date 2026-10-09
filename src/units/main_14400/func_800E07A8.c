#include "common.h"
typedef unsigned short u16;
typedef struct { s32 x; s32 y; } Pos;
typedef struct Unit { Pos pos; char pad8[0x16]; unsigned char field1E; } Unit;
u16 func_800E08F0(void *obj);
void func_800E0768(Unit *obj, s32 delta);
s32 func_80049CB4(s32 id, ...);
char *func_800A3B20(Unit *u);
void func_800497F0(s32, ...);
void func_800498E4(s32 id, ...);
static inline Pos *copy_position(Pos *out, Pos *from) { out->x = from->x; out->y = from->y; return out; }
s32 func_800E07A8(void *self, s32 amount) { Unit *obj = self; s32 delta; s32 message; Pos pos; delta = func_800E08F0(obj); func_800E0768(obj, amount); delta = func_800E08F0(obj) - delta; message = func_80049CB4(0xDA, copy_position(&pos, &obj->pos)); if (delta > 0) { if (obj->field1E & 0xC) func_800497F0(6, message, func_800A3B20(obj), delta); else func_800497F0(7, message, func_800A3B20(obj), delta); } else if (delta < 0) { if (obj->field1E & 0xC) func_800498E4(8, message, func_800A3B20(obj), delta); else func_800498E4(9, message, func_800A3B20(obj), delta); } return delta; }

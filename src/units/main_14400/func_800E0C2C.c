#include "common.h"
typedef unsigned short u16;
typedef struct { s32 x; s32 y; } Pos;
typedef struct { char pad0[0x68]; short adjust68; short pad6A; u32 (*call6C)(void *); short adjust70; short pad72; u32 (*call74)(void *); } VTable;
typedef struct Unit { Pos pos; char pad8[0x16]; unsigned char field1E; char pad1F[5]; VTable *vtable; char pad28[4]; u16 field2C; u16 field2E; } Unit;
s32 func_80049CB4(s32 id, ...);
char *func_800A3B20(Unit *u);
void func_800497F0(s32, ...);
static inline Pos *copy_position(Pos *out, Pos *from) { out->x = from->x; out->y = from->y; return out; }
static inline u16 low_half(u32 value) { return value; }
s32 func_800E0C2C(Unit *obj) { u32 previous; s32 message; Pos pos; previous = obj->vtable->call6C((char *)obj + obj->vtable->adjust68); if (low_half(previous ^ obj->vtable->call74((char *)obj + obj->vtable->adjust70))) { message = func_80049CB4(0xDA, copy_position(&pos, &obj->pos)); if (obj->field1E & 0xC) func_800497F0(0x21, message, func_800A3B20(obj)); else func_800497F0(0x22, message, func_800A3B20(obj)); obj->field2C = obj->field2E; return 1; } return 0; }

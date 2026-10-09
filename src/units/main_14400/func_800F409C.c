#include "common.h"
typedef unsigned char u8;
typedef unsigned short u16;
typedef struct Obj800E0F40 { char pad0[0xA]; u8 fieldA; } Obj800E0F40;
s32 func_800E0F40(Obj800E0F40 *obj);
s32 func_800F3FEC(u8, u8);
char *func_80048480(u16 id);
char *func_80083C90(char *dst, char *src);
char *func_800F409C(Obj800E0F40 *obj, char *out) { s32 kind = obj->fieldA; s32 id = func_800F3FEC(kind, func_800E0F40(obj)); func_80083C90(out, func_80048480(id)); return out; }

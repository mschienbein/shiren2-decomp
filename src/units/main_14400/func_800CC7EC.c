#include "common.h"
typedef unsigned char u8;
typedef unsigned short u16;
typedef struct { char pad0[4]; char text4[4]; u8 field8; u8 field9; u8 fieldA; } Obj;
extern char D_801C9CE0[];
extern char D_801C9CA0[];
char *func_80083F34(void *src, s32 len, char *dst);
char *func_800A8498(s32 id, s32 arg);
char *func_80048480(u16 id);
s32 func_800327C0(char *dst, const char *fmt, ...);
char *func_800CC7EC(Obj *obj) { char *name; if ((u32)(obj->field8 - 1) < 6) { if (obj->field9 == 0x17) { *func_80083F34(obj->text4, 4, D_801C9CE0) = 0; name = D_801C9CE0; } else name = func_800A8498(obj->field9, obj->fieldA); func_800327C0(D_801C9CA0, func_80048480(obj->field8 + 0x4E21), name); return D_801C9CA0; } return func_80048480(obj->field8 + 0x4E21); }

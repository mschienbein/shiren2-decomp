#include "common.h"
typedef unsigned char u8;
typedef unsigned short u16;
typedef struct { char pad[0x1E]; u8 flags; } Obj;
extern u8 D_80142F20;
extern char *func_800A3CD0(Obj *);
extern char *func_800A3B20(Obj *);
extern s32 func_800E4454(Obj *);
extern char *func_80048480(u16);
extern char *func_80083D04(char *, char *);
char *func_800A3D0C(Obj *p) {
    s32 isO = D_80142F20 == 0x4F;
    char *r;
    s32 c;
    if (isO) return func_800A3CD0(p);
    r = func_800A3B20(p);
    c = 0;
    if (p->flags & 0x7C) c = func_800E4454(p) != 0;
    if (c) func_80083D04(r, func_80048480(0x24C));
    return r;
}

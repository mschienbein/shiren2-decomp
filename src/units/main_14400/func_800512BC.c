#include "common.h"
typedef unsigned char u8;
typedef unsigned short u16;
typedef struct { s32 row; s32 col; } Cell;
typedef struct { s32 x, y, z; } Vec3i;
typedef struct { Cell cell; u8 b8; u8 b9; char padA[0x15]; u8 b1F; } Unit;
extern u8 D_80142F1B;
extern u8 D_80142F20;
extern u8 func_800A8C00(Unit *);
extern void func_80084A68(void);
extern s32 func_80048EE0(Unit *);
extern s32 func_800625FC(s32, s32);
extern s32 func_800627C4(void);
extern s32 func_80046240(void);
extern s32 func_80062554(s32, s32);
extern s32 func_800A99D0(void);
extern void *func_80085938(s32 id, s32 track, Vec3i pos, s32 arg4, s32 arg5, u16 flags, s32 arg7);
static inline s32 isDeepWater(s32 attr) {
    s32 wet = 0;
    if ((attr & 0x20100000) && func_800627C4() == 3) wet = (D_80142F1B & 3) == 2;
    return wet;
}
void func_800512BC(s32 owner, Unit *u, s32 flags, s32 arg3, s32 arg4) {
    s32 kind = func_800A8C00(u);
    Cell cell;
    u8 b;
    Vec3i scr;
    cell.row = u->cell.row;
    cell.col = u->cell.col;
    b = u->b8;
    func_80084A68();
    if (u->b1F == 0x17) flags |= 0x4000;
    if (func_80048EE0(u)) flags |= 0x20;
    {
        Cell here;
        s32 attr;
        here.row = u->cell.row;
        here.col = u->cell.col;
        attr = func_800625FC(here.col, here.row);
        if ((attr & 0x4000) || isDeepWater(attr)) flags |= 0x40;
    }
    if (flags & 0x80000) b = 0;
    else if (flags & 0x100000) b = 6;
    scr.x = cell.col * 32 + 16;
    scr.y = cell.row * 32 + 16;
    if (func_80046240()) {
        s32 isO;
        scr.z = func_80062554(cell.col, cell.row);
        isO = D_80142F20 == 0x4F;
        if (isO) {
            s32 below = 0;
            if ((u->b9 & 0xF) != 1) below = func_80062554(cell.col, cell.row) < 0;
            if (below) scr.z = 0;
        }
    } else if (arg4 && func_800A99D0()) {
        scr.z = func_80062554(cell.col, cell.row);
    } else {
        scr.z = ((u->b9 & 0xF) == 1) ? -0x20 : 0;
    }
    func_80085938(owner, kind, scr, arg3, b, flags, 0);
}

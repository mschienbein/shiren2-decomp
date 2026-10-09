#include "common.h"

typedef struct { unsigned char kind, variant, field_02, flags, row, field_05, field_06, field_07, mode; signed char coordinates[2], status; } SelectionRecord;
extern SelectionRecord D_80142F18;

typedef struct { s32 x, y; } Pos;
typedef struct { s32 ay, ax, by, bx; } Span;
typedef struct Unit Unit;
/* Effect task record returned by func_80085154. */
typedef struct { char pad[0x12]; unsigned short flags; char pad2[0x10]; s32 x24; s32 x28; char pad3[0x30]; s32 x5C; s32 x60; char pad4[4]; s32 x68; s32 x6C; } Task;
typedef void (*TaskFn)(void *task);
extern u32 D_8013968C;
void func_80088828(void *task);
void func_8008B9B0(void *task);
void func_8008AF00(void *task);
void func_80089BCC(void *task);
void func_8008865C(void *task);
unsigned char func_800A8C00(void *actor);
void *func_80085154(TaskFn handler, s32 value);
s32 func_800A99D0(void);
Unit *func_800C5F60(void);
void func_80049414(Unit *unit, u32 *flags, s32 *mode);
void func_80050CEC(s32 id, s32 flags, Pos *from, Pos *to, float scale);
void func_80084A20(void);
void func_800850F8(TaskFn handler, unsigned short value);
void *func_800851B0(s32 id);
void func_80084B80(void);
void func_8004CCD8(Unit *id, Pos *a, Pos *b) {
    s32 grp = func_800A8C00(id);
    Span sp;
    Task *t;
    s32 hide;
    sp.ay = a->y;
    sp.ax = a->x;
    sp.by = b->y;
    sp.bx = b->x;
    switch (D_8013968C) {
    case 0x17:
    case 0x18:
    case 0x19:
        t = func_80085154(func_80088828, grp);
        t->x5C = sp.ay;
        t->x68 = sp.ax;
        t->x60 = sp.by;
        t->x6C = sp.bx;
        hide = 0;
        if (func_800A99D0()) { s32 h = (D_80142F18.flags >> 2) & 1; hide = h == 0; }
        if (!hide) t->flags |= 0x100;
        if (D_8013968C == 0x18) t->flags |= 0x2000;
        if (id == func_800C5F60()) {
            t->flags |= 2;
            if (D_8013968C == 0x18) {
                u32 u;
                s32 v;
                func_80049414(id, &u, &v);
                t = func_80085154(func_8008B9B0, grp);
                t->x24 = u;
                t->x28 = v;
            }
        }
        break;
    case 0x9C:
        func_80085154(func_8008AF00, 0xA7);
        t = func_80085154(func_80088828, grp);
        t->x5C = sp.ay;
        t->x68 = sp.ax;
        t->x60 = sp.by;
        t->x6C = sp.bx;
        if (id == func_800C5F60()) t->flags |= 2;
        t->flags |= 0x1000;
        hide = 0;
        if (func_800A99D0()) { s32 h = (D_80142F18.flags >> 2) & 1; hide = h == 0; }
        if (!hide) t->flags |= 0x100;
        break;
    case 0x1B:
        func_80050CEC(0x15C, 0, a, b, 4.0f);
        break;
    /* ODD_C: effect id 0x8A is a listed id that spawns no task here; the label shapes codegen: without it 207 words differ (944 vs 952 bytes). */
    case 0x8A:
        break;
    case 0x8C: {
        s32 mine;
        t = func_80085154(func_80089BCC, grp);
        t->x5C = sp.ay;
        t->x68 = sp.ax;
        t->x60 = sp.by;
        t->x6C = sp.bx;
        {
            s32 normal = D_80142F18.mode != 0x4F;
            mine = 0;
            if (normal) mine = id == func_800C5F60();
        }
        if (mine) t->flags |= 2;
        break;
    }
    case 0x9E:
        func_80084A20();
        func_800850F8(func_8008865C, 4);
        func_800851B0(0x27);
        func_80084B80();
        t = func_80085154(func_80089BCC, grp);
        t->x5C = sp.ay;
        t->x68 = sp.ax;
        t->x60 = sp.by;
        t->x6C = sp.bx;
        t->flags |= 0x1000;
        break;
    case 0x9F:
        func_800851B0(0x7F);
        t = func_80085154(func_80089BCC, grp);
        t->x5C = sp.ay;
        t->x68 = sp.ax;
        t->x60 = sp.by;
        t->x6C = sp.bx;
        t->flags |= 0x2000;
        break;
    }
}

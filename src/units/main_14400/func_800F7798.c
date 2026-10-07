#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;

typedef struct { s32 x; s32 y; } Pos;
typedef struct { u8 data[0x18]; } Iter;
typedef struct { u8 pad[0x1E]; u8 flags; } Owner;
typedef struct { Owner *owner; s32 kind; } Info;
typedef struct { u8 pad[0x10]; Info *info; } Obj;
extern u8 D_801531A0[];
s32 func_800F438C(Pos *actor, Obj *event);
void *func_800C5280(Iter *it, Pos *pos, u16 flags);
s32 func_800C559C(Iter *it);
void func_800C532C(Pos *pos, Iter *it);
s32 func_800B56F0(Pos *pos);
void *func_800AC244(u8 id);
s32 func_800AD8AC(void *o, Pos *pos);
void func_800D3E28(s32 a, s32 b);
void func_800F7798(Pos *pos, Obj *obj) {
    Info *info;
    func_800F438C(pos, obj);
    info = obj->info;
    if (!(D_801531A0[info->kind] & 0x20)) {
        Iter it;
        Pos p;
        Pos *pp;
        p.x = pos->x;
        p.y = pos->y;
        func_800C5280(&it, &p, 2);
        pp = &p;
        while (func_800C559C(&it)) {
            s32 failed;
            void *o;
            func_800C532C(pp, &it);
            failed = func_800B56F0(pp) != 1;
            if (!failed) continue;
            o = func_800AC244(0xCB);
            if (o) func_800AD8AC(o, pp);
        }
    }
    if (info->owner != 0 && (info->owner->flags >> 2) & 1) func_800D3E28(0, 0);
}

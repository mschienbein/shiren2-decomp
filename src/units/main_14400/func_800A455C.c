#include "common.h"

typedef unsigned char u8;

typedef struct { s32 x; s32 y; } Pos;
typedef struct { u32 pad0 : 9; u32 flying : 1; u32 pad10 : 22; } Flags;
typedef struct { u8 value; } Dir;
typedef struct { Pos pos; Flags flags; Dir dir; } Request;
typedef struct { Pos pos; u8 pad8[0x18]; Flags flags; } Obj;
s32 func_800A282C(Pos *from, Request *req);
s32 func_800A23E8(Pos *from, Pos *to);
s32 func_800A674C(Obj *obj, Pos *dest);
void *func_800A27A4(Dir *dir, Pos *from, Pos *to);
s32 func_800A46BC(Obj *obj, Pos *pos, Dir *dir);
void func_800A2758(Pos *pos, Dir dir);
s32 func_800A455C(Obj *obj, Pos *dest, s32 maxSteps) {
    Pos cur;
    Pos target;
    Request req;
    Pos *p;
    s32 ok;
    s32 i;
    if (dest != 0) {
        p = &cur;
        cur.x = obj->pos.x;
        ok = 0;
        p->y = obj->pos.y;
        target.x = dest->x;
        target.y = dest->y;
        req.flags = obj->flags;
        req.pos.x = target.x;
        req.pos.y = target.y;
        if (func_800A282C(&cur, &req)) {
            req.pos.x = target.x;
            req.pos.y = target.y;
            if (func_800A23E8(&cur, &req.pos) <= maxSteps) {
                if (func_800A674C(obj, dest) != 0 || req.flags.flying) {
                    ok = 1;
                }
            }
        }
        if (ok) {
            req.pos.x = target.x;
            req.pos.y = target.y;
            func_800A27A4(&req.dir, &cur, &req.pos);
            for (i = 0; i < maxSteps; i++) {
                s32 failed = func_800A46BC(obj, &cur, &req.dir) != 1;
                if (failed) {
                    return 0;
                }
                func_800A2758(&cur, req.dir);
            }
            return 1;
        }
    }
    return 0;
}

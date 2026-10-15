#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

typedef struct { s32 x; s32 y; } Pos;
typedef struct { s32 x0; s32 y0; s32 x1; s32 y1; } Rect;
typedef struct { Pos pos; u8 pad8[4]; Rect room; u8 pad1C[2]; u8 flags1E; u8 pad1F[0xC5]; u16 flagsE4; } Target;
typedef struct {
    Pos pos;
    u8 pad8[0x50];
    Target *target;
    u8 pad5C[0x16];
    u8 flags72;
    u8 pad73[0xD];
    u8 kind;
    u8 pad81[0x19];
    u16 flags9A;
} Obj;
extern u8 D_80147620[];
extern u8 D_80148380[];
Target *func_800C5F60(void);
u8 func_800A6420(Obj *obj, Target *target);
s32 func_800C587C(void *arg0, u8 chance);
s32 func_800A31C8(Rect *room, Pos *pos);
s32 func_800A23E8(Pos *from, Pos *to);
s32 func_800A65B8(Obj *obj, Target *target);
u32 func_800B1C6C(Pos *pos);
void func_800E20F0(Obj *obj);
s32 func_800A4520(Obj *obj, Target *target);

void func_800F0A08(Obj *obj) {
    Pos tpos;
    Pos opos;
    Rect room;
    Pos tmp;
    Target *target = func_800C5F60();
    s32 ok = !((target->flags1E >> 2) & 1) || !(target->flagsE4 & 1);

    if (ok) {
        /* ODD_C: the sleep kind is kept twice, as an int for the 1..3 range
         * check and as a byte for every other test (see results.json). */
        s32 kind;
        u8 type;

        tpos.x = target->pos.x;
        tpos.y = target->pos.y;
        opos.x = obj->pos.x;
        opos.y = obj->pos.y;
        kind = obj->kind;
        type = kind;
        if (!func_800A6420(obj, target)) {
            s32 wake = 0;

            if ((kind >= 1 && kind <= 3) || type == 6
                || (type == 4 && func_800C587C(D_80147620, 0x32))
                || (type == 5 && func_800C587C(D_80147620, 0x4B))) {
                wake = 1;
            }
            if (wake) {
                func_800E20F0(obj);
            }
        } else {
            s32 inside;
            s32 wasInside;
            u16 flags;
            s32 bit;

            room.x0 = target->room.x0;
            room.y0 = target->room.y0;
            room.x1 = target->room.x1;
            room.y1 = target->room.y1;
            inside = func_800A31C8(&room, &opos);
            flags = obj->flags9A;
            bit = flags & 0x10;
            wasInside = bit != 0;
            obj->flags9A = inside ? (flags | 0x10) : (flags & ~0x10);
            switch (type) {
            case 2:
                if (inside) {
                    tmp.x = tpos.x;
                    tmp.y = tpos.y;
                    if (func_800A23E8(&opos, &tmp) < 4) {
                        func_800E20F0(obj);
                    }
                }
                break;
            default:
                if (type < 3 || type > 6) {
                    break;
                }
                if (wasInside == inside) {
                    s32 reachable = 0;

                    if (func_800A65B8(obj, target) == 1) {
                        u32 bits = func_800B1C6C(&tpos) & 0x4000;
                        reachable = bits == 0;
                    }
                    if (!reachable) {
                        break;
                    }
                }
                if (func_800C587C(D_80147620, D_80148380[type - 3])) {
                    func_800E20F0(obj);
                }
                break;
            }
        }
        {
            s32 clear = (obj->flags72 & 1) ^ 1;

            if (clear && func_800A4520(obj, target)) {
                obj->target = target;
            }
        }
    }
}

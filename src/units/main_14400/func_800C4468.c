#include "common.h"
typedef unsigned char u8;
typedef unsigned short u16;
typedef struct { s32 x, y; } Point;
typedef struct { u8 value; } Dir;
typedef struct { u8 pad_00[0x1E]; u8 flags_1E; } Unit;
typedef struct {
    Point pos; Dir direction; u8 pad_09[0xF]; u8 active_18; u8 pad_19[7];
    Unit *owner_20; void *item_24; u16 name_28; u8 pad_2A[2]; Point origin_2C;
    s32 action_34; u16 flags_38; u8 flags_3A; u8 pad_3B;
    Unit *reflected_3C; Unit *hit_40; Point last_44; Dir last_dir_4C;
} Projectile;
extern u8 D_80156A09;
extern s32 func_800A6FD0(void *), func_80049CB4(s32, ...);
extern u32 func_800B1C6C(Point *);
extern Point *func_800A25D8(Point *, Point *, Dir, s32);
extern void func_800A2758(Point *, Dir), func_800C47C0(void *, Point *, Point *);
extern s32 func_800B5650(Point *), func_8005DFE8(u8 *);
extern void func_800497F0(s32, ...), func_80049C90(s32, s32), func_800E20F0(void *);
extern char *func_80048480(u16), *func_800A3B20(void *);
extern s32 func_800B5300(void *, void *, u8);
extern Unit *func_800B4928(Point *);
extern s32 func_800A5440(void *, void *, u8 *, s32, char *);
static inline Dir opposite(Dir *direction) { Dir result; result.value = (direction->value + 4) & 7; return result; }
s32 func_800C4468(Projectile *self, s32 action, Point *position, Dir direction) {
    Point end;
    s32 message;
    s32 result;
    Unit *hit;
    self->last_44 = *position;
    self->last_dir_4C = direction;
    if (self->owner_20 && func_800A6FD0(self->owner_20)) message = -1;
    else message = func_80049CB4(0xDA, position);
    switch (action) {
    case 0:
        if (func_800B1C6C(position) & 0x4000) return 1;
        func_800A25D8(&end, position, direction, 10);
        *position = end;
        func_800C47C0(self, &self->origin_2C, position);
        return 0;
    case 1:
    case 6:
        if (action == 1 && (self->flags_38 & 0x800)) func_800A2758(position, opposite(&direction));
        func_800C47C0(self, &self->origin_2C, position);
        return 0;
    case 3:
        if (self->flags_38 & 0x800) return 1;
        func_800C47C0(self, &self->origin_2C, position);
        if (self->flags_38 & 0x2000) {
            func_800B5650(position);
            func_800497F0(0x108, message);
        } else {
            func_80049CB4(0x116, position);
            func_800497F0(0x104, message, func_80048480(self->name_28));
            func_80049C90(1, message);
            func_800B5300(position, 0, D_80156A09);
            func_800497F0(0x107, message);
        }
        return 0;
    case 4:
        func_800C47C0(self, &self->origin_2C, position);
        hit = func_800B4928(position);
        switch (func_800A5440(hit, self->owner_20, &direction.value, self->flags_38, func_80048480(self->name_28))) {
        case 1: result = 0; break;
        case 2:
            direction = opposite(&direction);
            self->direction = direction;
            func_800A2758(position, direction);
            self->pos = *position;
            self->active_18 = 1;
            self->origin_2C = *position;
            self->reflected_3C = hit;
            self->flags_38 |= 0x10;
            result = 1;
            if (self->action_34 == 0xEE) self->action_34 = 0xEF;
            break;
        default:
            if (!(self->flags_3A & 1)) {
                char *name;
                if (func_800A6FD0(hit)) message = -1;
                name = func_80048480(self->name_28);
                if (func_8005DFE8((u8 *)name) < 0x34)
                    func_800497F0(0x63, message, name, func_800A3B20(hit));
                else
                    func_800497F0(0x64, message, name, func_800A3B20(hit));
            }
            self->hit_40 = hit;
            result = 0;
            break;
        }
        if (hit->flags_1E & 0x7C) func_800E20F0(hit);
        return result;
    default:
        return 1;
    }
}

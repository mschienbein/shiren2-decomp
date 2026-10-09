#include "common.h"
typedef unsigned char u8;
typedef unsigned short u16;
typedef struct { s32 x, y; } Position;
typedef struct { u8 value; } Dir;
typedef struct { u8 storage[0x20]; } Member;
typedef struct { u8 pad_0[0xC]; Member field_C; u8 field_2C; } Trigger;
typedef struct { Position pos; u8 pad_8[2]; u8 field_A; u8 pad_B[0x13]; u8 field_1E; u8 pad_1F[0x5D]; u16 field_7C; u8 pad_7E[0xE]; void *field_8C; } Obj;
typedef Obj Unit;
typedef Obj Obj800A46BC;
extern s32 func_800CD278(Member *m);
extern void *func_800A27A4(void *out, void *from, void *to);
extern s32 func_800A4754(Obj800A46BC *obj, void *arg, Dir *cell);
extern u8 func_800A8C00(void *obj);
extern s32 func_80121848(Trigger *trigger, Obj *obj);
extern char *func_800A3B20(Unit *obj);
extern char *func_800A3CD0(Obj *obj);
extern void func_800498E4(s32 id, ...);
extern s32 func_80049CB4(s32 id, ...);
extern s32 func_800CF47C(void *obj);
s32 func_80121940(Trigger *trigger, Obj *obj, Position *target, s32 forced) {
    Position origin, destination;
    Position *origin_ptr;
    Dir dir;
    s32 eligible = 0;
    origin.x = obj->pos.x;
    origin_ptr = &origin;
    origin_ptr->y = obj->pos.y;
    if (((obj->field_1E >> 4) & 1) && !((obj->field_7C >> 9) & 1) && func_800CD278(&trigger->field_C) > 0) {
        u8 kind;
        if (!forced) {
            destination.x = target->x;
            destination.y = target->y;
            func_800A27A4(&dir, origin_ptr, &destination);
            if (!func_800A4754(obj, origin_ptr, &dir)) goto tested;
        }
        kind = trigger->field_2C;
        if (kind == 0xFF || kind == func_800A8C00(obj)) eligible = 1;
    }
tested:
    if (eligible) {
        if (!func_80121848(trigger, obj)) return 0;
        if (forced) {
            func_800498E4(0xA3, func_800A3B20(obj));
            func_80049CB4(6);
            func_80049CB4(0x4109, &origin);
            func_80049CB4(6);
        } else {
            func_800498E4(0xA7, func_800A3CD0(obj));
            func_80049CB4(0x109E, obj, &origin, target);
        }
        func_80049CB4(0x87, obj);
        if (obj->field_8C && obj->field_A != 0x3D) func_800CF47C(obj->field_8C);
        return 1;
    }
    return 0;
}

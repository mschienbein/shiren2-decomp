#include "common.h"
typedef unsigned char u8;
typedef unsigned short u16;
typedef struct { s32 x, y; } Position;
typedef struct { u8 v; } Dir;
typedef struct { Position position; Dir direction; } Object;
typedef struct { void *source; u32 kind; u32 field_8; u16 amount; u16 flags; u8 field_10; } Damage;

s32 func_800EE504(void *object);
void func_800E8FAC(void *object);
s32 func_800E1CC4(void *object, s32 kind);
void func_800A665C(void *obj, u8 *value);
void *func_800A6CC0(void *out_position, void *obj);
s32 func_800B5900(void *pos, s32 mode, s32 arg, void **out);
void func_800A2758(Position *p, Dir d);
s32 func_80049CB4(s32 id, ...);
char *func_800A3B20(void *obj);
void func_800498E4(s32 id, ...);
s32 func_800E0F40(void *object);
/* receiver: unused by the callee; this caller passes the object. */
u8 *func_80044D74(void *receiver, u8 index);
void func_80136910(Damage *damage, void *source, u32 amount, u32 kind, u32 flags);
void func_800A7ADC(void *target, Damage *damage);

static inline Damage *make_damage(Damage *damage, void *source, u32 amount) {
    func_80136910(damage, source, amount, 2, 0x808);
    return damage;
}

/* Monster slot +0xB4: the target pointer may be replaced by func_800B5900. */
s32 func_80109DE4(Object *object, void *target) {
    Position origin, destination, probe;
    Damage damage;
    Dir direction;
    s32 status = func_800EE504(object) ^ 1;
    if (status) {
        func_800E8FAC(object);
        return 1;
    }
    origin.x = object->position.x;
    origin.y = object->position.y;
    direction = object->direction;
    if (func_800E1CC4(object, 4)) {
        direction.v = (direction.v + 4) & 7;
        func_800A665C(object, &direction.v);
    }
    func_800A6CC0(&destination, object);
    for (;;) {
        probe.x = destination.x;
        probe.y = destination.y;
        if (!func_800B5900(&probe, 2, 2, &target)) break;
        func_800A2758(&destination, direction);
    }
    func_80049CB4(0x6B, object);
    func_80049CB4(0x100, &origin, &destination);
    func_800498E4(0x61, func_800A3B20(object));
    if (target) {
        u8 *metadata = func_80044D74(object, func_800E0F40(object));
        func_800A7ADC(target, make_damage(&damage, object, metadata[6]));
    }
    return 1;
}

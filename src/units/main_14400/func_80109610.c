#include "common.h"

typedef unsigned char u8;
typedef short s16;
typedef struct {
    s32 x;
    s32 y;
} Pos;
typedef struct {
    u8 data[0x18];
} Buffer;
typedef union {
    Pos pos;
    s16 result;
} Scratch;
typedef struct Object {
    Pos pos;
    u8 dir;
} Object;
typedef struct {
    u8 pad_0[6];
    u8 field_6;
    u8 field_7;
} Parameters;

extern s32 func_800E0F40(Object *obj);
/* The original passes the same object pointer in a0; this is an unused receiver. */
extern void *func_80044DCC(void *receiver, u8 index);
extern s32 func_800EE504(Object *obj);
extern void func_800E8FAC(Object *obj);
extern s32 func_800E20CC(void *arg0);
extern s32 func_80109370(void *arg0, void *arg1, s32 arg2);
extern void *func_800A39C0(void *self, void *position, unsigned char *direction, s32 distance);
extern s32 func_800E1CC4(Object *obj, s32 kind);
extern void func_800A665C(Object *obj, u8 *value);
extern Pos *func_800A256C(Pos *out, Pos *a, Pos *b);
extern Pos *func_800A2544(Pos *out, Pos *a, Pos *b);
extern u32 func_800B1C6C(Pos *pos);
extern void *func_800B4928(Pos *pos);
extern void *func_800A6538(void *out_direction, void *obj, void *target);
extern s32 func_80049CB4(s32 id, ...);
extern void func_80136910(Buffer *obj, void *a, u32 c, u32 b, u32 e);
extern void func_800A7A9C(Object *obj, Buffer *message, s16 *result);

static inline void turn_toward(Object *actor, Pos *target)
{
    u8 face;

    func_800A6538(&face, actor, target);
    func_800A665C(actor, &face);
}

static inline void init_attack(Buffer *message, Object *source, u8 power)
{
    func_80136910(message, source, power, 1, 8);
}

s32 func_80109610(Object *actor, Object *other)
{
    Pos origin;
    Pos target;
    Scratch tmp;
    Pos sum;
    Buffer message;
    u8 dir;
    u8 back;
    Parameters *params;

    origin.x = actor->pos.x;
    origin.y = actor->pos.y;
    dir = actor->dir;
    params = func_80044DCC(actor, func_800E0F40(actor));
    if ((func_800EE504(actor) ^ 1) != 0) {
        func_800E8FAC(actor);
        return 1;
    }
    if (func_800E20CC(actor)) {
        other = 0;
    } else if (other != 0 && (func_80109370(actor, other, params->field_7) ^ 1) != 0) {
        other = 0;
    }
    if (other != 0) {
        target = other->pos;
    } else {
        if (!func_800E20CC(actor)) {
            return 0;
        }
        func_800A39C0(&tmp.pos, actor, &dir, params->field_7);
        target = tmp.pos;
    }
    if (func_800E1CC4(actor, 4)) {
        back = (dir + 4) & 7;
        func_800A665C(actor, &back);
        func_800A256C(&tmp.pos, &target, &actor->pos);
        tmp.pos.y = -tmp.pos.y;
        tmp.pos.x = -tmp.pos.x;
        func_800A2544(&sum, &actor->pos, &tmp.pos);
        tmp.pos = sum;
        target = tmp.pos;
        if (!(func_800B1C6C(&target) & 0x4000)) {
            other = func_800B4928(&target);
        } else {
            other = 0;
        }
    } else if (other == 0) {
        return 0;
    }
    turn_toward(actor, &target);
    func_80049CB4(0x38, actor);
    func_80049CB4(0x10D, &origin, &target);
    if (other != 0) {
        init_attack(&message, actor, params->field_6);
        tmp.result = 0;
        func_800A7A9C(other, &message, &tmp.result);
    }
    return 1;
}

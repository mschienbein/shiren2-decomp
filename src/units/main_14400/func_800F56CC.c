#include "common.h"
typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef struct { s32 x; s32 y; } Point;
typedef Point Pos;
typedef Point Tmp800A46BC;
typedef struct { u8 value; } Dir;
typedef struct { u32 bits; } Flags;
typedef struct { Point position; u8 field_8; u8 pad9[0x15]; u8 field_1E; u8 pad1F; Flags field_20; } Object;
typedef Object Obj;
typedef Object Obj_80049414;
typedef struct { void *field_0; u32 field_4; u32 field_8; u16 field_C; u16 field_E; u8 field_10; } Obj80136910;
typedef Obj80136910 Message;
typedef struct { u16 field_0; u16 field_2; s32 field_4; } Result;
extern u16 D_80156A36;
extern void *func_800A2594(Tmp800A46BC *out, void *arg, Dir cell);
extern void *func_800B4928(Point *pos);
extern s32 func_800A455C(Obj *obj, Pos *dest, s32 maxSteps);
extern s32 func_800E1CC4(Obj_80049414 *obj, s32 kind);
extern void func_800A665C(Obj *obj, u8 *value);
extern s32 func_80049CB4(s32 id, ...);
extern void func_80136910(Obj80136910 *obj, void *a, u32 c, u32 b, u32 e);
extern void func_800A7A9C(Object *, Message *, Result *);
extern void func_800A2F80(Dir *, s32);
static inline void copy_position(Point *out, Object *obj) { out->x = obj->position.x; out->y = obj->position.y; }
static inline void copy_flags(Flags *out, Object *obj) { out->bits = obj->field_20.bits; }
static inline void deliver(Object *target, Object *source, s32 value, Result *result) {
    Message message;
    func_80136910(&message, source, value, 1, 8);
    result->field_0 = 0;
    func_800A7A9C(target, &message, result);
}
void func_800F56CC(Object *obj) {
    Point origin;
    union { Point position; Result result; } cell;
    Flags flags;
    Dir direction;
    s32 i = 0;
    s32 value;
    copy_position(&origin, obj);
    value = (u32)D_80156A36 << 16;
    direction.value = obj->field_8;
    for (;;) {
        s32 more = i < 8;
        s32 eligible;
        Object *target;
        if (!more) break;
        func_800A2594(&cell.position, &origin, direction);
        eligible = 0;
        target = func_800B4928(&cell.position);
        if (func_800A455C(obj, (Pos *)target, 1) && (target->field_1E & 0x7C) && !func_800E1CC4(target, 1)) {
            s32 blocked;
            copy_flags(&flags, target);
            blocked = (flags.bits >> 23) & 1;
            eligible = blocked == 0;
        }
        if (eligible) {
            func_800A665C(obj, &direction.value);
            func_80049CB4(0xB4, obj);
            deliver(target, obj, value >> 16, &cell.result);
        }
        func_800A2F80(&direction, 1);
        i++;
    }
}

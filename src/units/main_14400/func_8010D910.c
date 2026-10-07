#include "common.h"
typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef struct { u8 value; } Dir;
typedef struct Message {
    void *field_0;
    s32 field_4;
    s32 field_8;
    s16 field_C;
    u16 field_E;
    u8 field_10;
} Message;
typedef struct { s32 x, y; } Point;
typedef struct { Point field_0; unsigned char field_8; unsigned char field_9[0x13]; unsigned short field_1C; unsigned char field_1E; } Object;
typedef struct { s32 field_0; Object *field_4; } Event;
typedef struct { short field_0, field_2; s32 field_4; } Result;
extern s32 func_80049CB4(s32, ...);
extern s32 func_8010CD1C(void *), func_800E8DC8(Object *, s16);
extern void func_80136910(Message *, Object *, u32, u32, u32);
extern s32 func_800A46BC(Object *, Point *, unsigned char *);
extern s32 func_8010BEC4(void *, u8);
extern void func_800A2758(Point *, Dir);
extern Object *func_800B4928(Point *);
extern s32 func_800A674C(Object *, Object *);
extern void func_800A7A9C(Object *, Message *, Result *);
extern void *func_800A6CC0(Point *, Object *);
extern s32 func_800FCF3C(Point *, s32);
static inline void position(Point *out, Object *arg) { out->x = arg->field_0.x; out->y = arg->field_0.y; }
void func_8010D910(void *arg, Event *event)
{
    Object *target = event->field_4;
    if (target->field_1E & 0xC) {
        Message message;
        Point point;
        Result result;
        Point destination;
        Dir direction;
        s32 blocked;
        Object *other;
        s32 valid;
        func_80049CB4(0x47, target, 1, 0);
        func_80136910(&message, target, (u32)(s16)func_800E8DC8(target, (s16)func_8010CD1C(arg)), 1, 0);
        blocked = 0;
        position(&point, target);
        direction.value = target->field_8;
        if (!func_800A46BC(target, &point, &direction.value) && !(u8)func_8010BEC4(arg, 0x78)) blocked = 1;
        if (!blocked) {
            func_800A2758(&point, direction);
            other = func_800B4928(&point);
            valid = 0;
            if (other && !(other->field_1C & 1)) valid = func_800A674C(target, other) != 0;
            if (valid) {
                result.field_0 = 0;
                func_800A7A9C(other, &message, &result);
            }
            if ((u8)func_8010BEC4(arg, 0x19)) {
                func_800A6CC0(&destination, target);
                func_800FCF3C(&destination, 1);
            }
        }
    }
}

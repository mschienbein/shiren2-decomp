#include "common.h"
typedef unsigned char u8;
typedef unsigned short u16;
typedef signed short s16;
typedef struct { u8 value; } Dir;
typedef struct { s32 x, y; } Point;
typedef struct { u32 bits; } Flags;
typedef struct { Point field_0; unsigned char field_8; unsigned char field_9[0x13]; unsigned short field_1C; unsigned short field_1E; Flags field_20; } Object;
typedef struct Message {
    void *field_0;
    s32 field_4;
    s32 field_8;
    s16 field_C;
    u16 field_E;
    u8 field_10;
} Message;
typedef struct { unsigned short field_0, field_2; s32 field_4; } Result;
extern signed char D_80148300[];
extern s32 func_80049CB4(s32, ...);
extern void func_800A2F80(unsigned char *, s32);
extern s32 func_800A46BC(Object *, Point *, unsigned char *);
extern void func_800A2758(void *, Dir);
extern Object *func_800B4928(Point *);
extern s32 func_800A674C(Object *, Object *);
extern void func_800A7A9C(Object *, Message *, Result *);
static inline void position(Point *out, Object *arg) { out->x = arg->field_0.x; out->y = arg->field_0.y; }
static inline void read_flags(Flags *out, Object *arg) { out->bits = arg->field_20.bits; }
static inline s32 result_blocked(Result *result) { return (result->field_0 >> 3) & 1; }
static inline s32 message_continues(Message *message) { return (message->field_E >> 6) & 1; }
s32 func_800E33D0(Object *arg, Message message, u8 rays, u8 range)
{
    Point point;
    Result result;
    Flags flags;
    Dir direction;
    unsigned char limit = range;
    s32 hit;
    s32 i;
    s32 unlimited;
    message.field_0 = arg;
    message.field_4 = 1;
    func_80049CB4(0x47, arg, limit, message.field_E);
    hit = 0;
    i = 0;
    func_80049CB4(6);
    read_flags(&flags, arg);
    unlimited = (flags.bits >> 22) & 1;
    direction.value = arg->field_8;
    for (;;) {
        s32 more = i < rays;
        s32 j;
        if (!more) break;
        func_800A2F80(&direction.value, D_80148300[i]);
        position(&point, arg);
        j = 0;
        for (;;) {
            s32 more = j < limit;
            s32 failed;
            s32 ignored;
            Object *target;
            if (!more) break;
            failed = func_800A46BC(arg, &point, &direction.value) != 1;
            if (failed) break;
            func_800A2758(&point, direction);
            ignored = 0;
            target = func_800B4928(&point);
            if (!target || (!unlimited && !func_800A674C(arg, target)) || (target->field_1C & 1)) ignored = 1;
            if (!ignored) {
                result.field_0 = 0;
                func_800A7A9C(target, &message, &result);
                failed = result_blocked(&result) != 1;
                if (failed) hit = 1;
                failed = message_continues(&message) != 1;
                if (failed) break;
            }
            j++;
        }
        i++;
    }
    func_80049CB4(7);
    return hit;
}

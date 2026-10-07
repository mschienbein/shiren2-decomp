#include "common.h"
typedef unsigned char u8;
typedef unsigned short u16;
typedef struct { u8 value; } Dir;
typedef struct { s32 x, y; } Point;
typedef struct {
    unsigned char field_0[0x10];
    short field_10;
    s32 (*field_14)(void *);
    unsigned char field_18[0x30];
    short field_48;
    s32 (*field_4C)(void *, Point *);
} VTable;
typedef struct Object {
    Point field_0;
    unsigned char field_8[0x14];
    unsigned short field_1C;
    unsigned char field_1E, field_1F[5];
    VTable *field_24;
    unsigned char field_28[0x2E];
    unsigned char field_56, field_57[0x43];
    unsigned short field_9A;
    s32 field_9C;
    struct Object *field_A0;
} Object;
extern s32 func_800E20CC(void *obj), func_800A6E90(Object *);
extern u32 func_800B1C6C(void *pos);
extern unsigned char func_800A6420(Object *, Object *);
extern s32 func_800A4520(Object *, Object *), func_800A692C(Object *, s32);
extern void func_80104040(Point *, Object *, Object *);
extern void *func_800A65E4(void *out_direction, void *obj, void *target);
extern void func_800A6690(Object *, unsigned char *, s32);
extern s32 func_800A251C(Point *, void *), func_800B4FF0(Point *, s32), func_800E1CC4(Object *, s32);
extern void func_800A59A4(Object *);
extern s32 func_80049CB4(s32, ...);
extern Dir *func_800A22B8(Dir *, void *, void *);
extern void func_800A665C(Object *, unsigned char *);
extern s32 func_800A4314(Object *, Point *), func_800B4888(Point *);
extern Object *func_800B4928(Point *);
extern char *func_800A3B20(void *obj);
extern s32 func_800A5440(Object *, Object *, unsigned char *, s32, char *);
extern void func_80049AE8(s32 message_id, ...);
extern void func_800A7B18(Object *, Object *, s32, s32);
extern void *func_800A2594(void *out, void *from, Dir dir);
extern s32 func_800A5D2C(void *obj, void *position, u16 flags);
extern s32 func_800A529C(void *obj, s32 flag);
extern void func_800A58FC(Object *, Point *);
extern s32 func_800A7A1C(void *placement);
extern void func_800A7BA4(Object *, s32), func_800E3678(Object *, Object *);
static inline void position(Point *out, Object *arg) { out->x = arg->field_0.x; out->y = arg->field_0.y; }
static inline s32 state(Object *arg) { return arg->field_24->field_14((unsigned char *)arg + arg->field_24->field_10); }
static inline void validate_destination(Object *arg, Point *destination)
{
    s32 failed = func_800A5D2C(arg, destination, 10) != 1;
    if (failed) { destination->x = 0; destination->y = 0; }
}
s32 func_8010457C(Object *arg, Object *provided)
{
    Point original, next, destination, temporary;
    unsigned char orientation;
    Dir direction;
    Object *subject;
    s32 rejected;
    s32 message;
    s32 immediate;
    s32 failed;
    s32 mode = func_800E20CC(arg);
    subject = provided;
    if (mode) {
        rejected = 0;
        if (!subject || (func_800B1C6C(subject) & 0x4000) || (subject->field_1C & 1) || func_800A6E90(subject)) rejected = 1;
        if (rejected) return 0;
    } else {
        subject = arg->field_A0;
        if (func_800A6420(arg, subject)) {
            rejected = 0;
            if (!provided || !func_800A4520(arg, provided)) rejected = 1;
            return rejected;
        }
        if (subject->field_1C & 1) return 0;
    }
    if (func_800A692C(arg, 0x12)) {
        rejected = 0;
        if (func_800A4520(arg, provided) || func_800E20CC(arg)) rejected = 1;
        return rejected ^ 1;
    }
    position(&original, arg);
    func_80104040(&next, arg, subject);
    func_800A65E4(&orientation, arg, subject);
    func_800A6690(arg, &orientation, 1);
    rejected = 0;
    if (!(next.y | next.x) || func_800A251C(&next, &original) ||
        (func_800A251C(&next, subject) && !func_800E20CC(arg) &&
         (!(arg->field_9A & 0x40) || !func_800B4FF0(&next, 0x10)))) rejected = 1;
    if (rejected) return func_800A4520(arg, provided) ^ 1;
    if (func_800E1CC4(arg, 4)) next = subject->field_0;
    func_800A59A4(subject);
    if ((subject->field_1E & 0x7C) && !(subject->field_1E & 0xC)) subject->field_56 = 0;
    func_80049CB4(0x105D, arg, subject);
    func_800A22B8(&direction, &original, &next);
    func_800A665C(arg, &direction.value);
    message = func_80049CB4(0x5E, arg);
    func_80049CB4(0x8C, subject, &original, &next);
    if (func_800B1C6C(&next) & 0x2000) func_80049CB4(0x10B, &next);
    immediate = 0;
    destination.x = 0;
    destination.y = 0;
    if (func_800A4314(subject, &next)) {
        destination = next;
        immediate = 1;
    } else if (func_800B4888(&next)) {
        char *subject_id;
        s32 collision;
        provided = func_800B4928(&next);
        subject_id = func_800A3B20(subject);
        collision = func_800A5440(provided, arg, &direction.value, 1, subject_id);
        if (collision == 2) {
            direction.value = (direction.value + 4) & 7;
            func_80049CB4(0x8C, subject, &next, &original);
            next = original;
        } else if (collision != 1) {
            s32 clear_mask;
            subject->field_0 = next;
            func_80049AE8(0x64, message, subject_id, func_800A3B20(provided));
            provided->field_1C |= 0x100;
            func_800A7B18(provided, arg, 5, 0x26);
            clear_mask = ~0x100;
            provided->field_1C &= clear_mask;
            subject->field_1C |= 0x100;
            func_800A7B18(subject, arg, 5, 0x25);
            subject->field_1C &= clear_mask;
        }
        destination = next;
        validate_destination(subject, &destination);
    } else if (func_800B1C6C(&next) & 0x4000) {
        subject->field_0 = next;
        func_800A7B18(subject, arg, 5, 0x25);
        failed = state(subject) != 1;
        if (failed) {
            s32 heading = direction.value;
            Dir turn;
            if ((heading ^ 1) & 1) {
                turn.value = (heading + 4) & 7;
                func_800A2594(&temporary, &next, turn);
                destination = temporary;
            } else {
                s32 rejected;
                turn.value = (heading + 2) & 7;
                func_800A2594(&temporary, &next, turn);
                destination = temporary;
                rejected = 0;
                if (!func_800A4314(subject, &destination) ||
                    !arg->field_24->field_4C((unsigned char *)arg + arg->field_24->field_48, &destination)) rejected = 1;
                if (rejected) {
                    turn.value = (direction.value - 2) & 7;
                    func_800A2594(&temporary, &next, turn);
                    destination = temporary;
                }
            }
        }
        failed = func_800A4314(subject, &destination) != 1;
        if (failed) {
            destination = next;
            validate_destination(subject, &destination);
        }
    }
    failed = state(subject) != 1;
    if (failed) {
        if (!(destination.y | destination.x)) {
            subject->field_0 = next;
            func_800A529C(subject, 0);
            return 1;
        }
        if (!immediate) func_80049CB4(0x8C, subject, &next, &destination);
        func_800A58FC(subject, &destination);
        func_80049CB4(0x1F, subject);
        func_80049CB4(0xD9);
        func_800A7A1C(subject);
        func_800A7BA4(subject, 6);
        if (subject->field_1E & 0x7C) func_800E3678(subject, arg);
    }
    return 1;
}

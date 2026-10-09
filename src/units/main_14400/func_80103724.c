#include "common.h"
typedef struct { unsigned char kind, variant, field_02, flags, row, field_05, field_06, field_07, mode; signed char coordinates[2], status; } SelectionRecord;
extern SelectionRecord D_80142F18;

typedef unsigned char u8;
typedef unsigned short u16;
typedef struct { s32 x, y; } Point;
typedef struct { Point min, max; } Rect;
typedef struct { Point current, start, end; } Iter;
typedef union { u16 value; struct { u8 high, low; } bytes; } Flags;
typedef struct { u8 pad_00[0x10]; short delta_10; short index_12; s32 (*method_14)(void *); } Methods;
typedef struct Obj Obj;
struct Obj { Point field_00; u8 field_08, field_09; u8 pad_0A[0x12]; Flags field_1C; u8 field_1E; u8 pad_1F[5]; Methods *field_24; u8 pad_28[0x30]; Obj *field_58; u8 pad_5C[0x16]; u8 field_72; };

extern s32 func_800E0F40(Obj *obj);
/* Returns its rectangle by value through the hidden result pointer. */
extern Rect func_800B3080(void *pos);
extern s32 func_80049CB4(s32 id, ...);
extern char *func_800A3B20(Obj *obj);
extern void func_800497F0(s32 id, ...);
extern void func_800AC6F8(Rect *rect, s32 allow_special);
extern s32 func_800A8F6C(s32 *iterator);
extern void *func_800A910C(s32 *iterator);
extern Point *func_800A3610(Point *out, Iter *iterator);
extern void *func_800B4928(Point *pos);
extern u32 func_800B1C6C(Point *pos);
extern s32 func_800A5D2C(void *obj, Point *out, s32 flags);
extern void func_800E20F0(Obj *obj);
extern void func_800A58FC(void *actor, Point *pos);
extern s32 func_800B4FF0(void *pos, s32 kind);
extern s32 func_800A7A1C(Obj *obj);
extern void func_800A7BA4(Obj *obj, s32 value);
s32 func_80103724(Obj *obj, Obj *other)
{
    Point origin;
    Rect range;
    Iter it;
    Point current;
    Point destination;
    s32 list_iterator;
    s32 event;
    s32 allowed;
    s32 perform;
    Obj *target;
    origin.x = obj->field_00.x;
    origin.y = obj->field_00.y;
    if (other != 0) {
        s32 same = 0;
        if (other == obj->field_58) {
            same = (u8)func_800E0F40(obj) == 1;
        }
        if (same) {
            return 0;
        }
    }
    range = func_800B3080(&origin);
    event = func_80049CB4(0x60, obj);
    perform = D_80142F18.mode != 0x4F;
    if (perform) {
        func_800497F0(0x146, event, func_800A3B20(obj));
        if ((u8)func_800E0F40(obj) != 2) {
            func_800AC6F8(&range, (u8)func_800E0F40(obj) == 3);
        }
        if ((u8)func_800E0F40(obj) == 1) {
            return 1;
        }
        if ((u8)func_800E0F40(obj) != 1) {
            list_iterator = 0;
            while (func_800A8F6C(&list_iterator)) {
                Obj *entry = func_800A910C(&list_iterator);
                entry->field_1C.value &= 0x7FFF;
            }
        }
        current.x = range.min.x;
        current.y = range.min.y;
        it.start = current;
        it.current = it.start;
        current.x = range.max.x;
        current.y = range.max.y;
        it.end = current;
        while (1) {
            s32 more = it.current.x <= it.end.x;
            if (!more) {
                break;
            }
            func_800A3610(&current, &it);
            target = func_800B4928(&current);
            allowed = 0;
            if (target != 0 &&
                target->field_24->method_14((u8 *)target + target->field_24->delta_10) == 0 &&
                !(target->field_1C.value & 1) &&
                ((target->field_09 & 15) != 1 || !(func_800B1C6C(&current) & 0x80)) &&
                target != obj && (target->field_1C.bytes.high >> 7) == 0) {
                allowed = 1;
            }
            if (!allowed) {
                continue;
            }
            destination.x = current.x;
            destination.y = current.y;
            allowed = 0;
            if (((target->field_1E >> 6) & 1) || func_800A5D2C(target, &destination, 1)) {
                allowed = 1;
            }
            if (!allowed) {
                continue;
            }
            {
                s32 notify = 0;
                if (target->field_1E & 0x7C) {
                    notify = target->field_72 & 1;
                }
                if (notify) {
                    func_800E20F0(target);
                }
            }
            func_80049CB4(6);
            func_80049CB4(0x8C, target, &current, &destination);
            func_800A58FC(target, &destination);
            func_80049CB4(7);
            if (func_800B4FF0(&destination, 0x10)) {
                func_80049CB4(0x132);
                func_800A7A1C(target);
            } else {
                func_800A7BA4(target, 6);
            }
            target->field_1C.value |= 0x8000;
        }
    }
    return 1;
}

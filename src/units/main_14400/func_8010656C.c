#include "common.h"

typedef struct { s32 x, y; } Point;
typedef struct Object Object;
struct Object {
    Point unk00;
    unsigned char unk08[0x50];
    Object *unk58; /* optional target unit */
    unsigned char unk5C[8];
    Point unk64;
    unsigned char unk6C[6];
    unsigned char unk72;
    unsigned char unk73[0x17];
    unsigned char unk8A;
};
extern u32 func_800B1C6C(Point *);
extern unsigned char func_800A6420(Object *, Object *);
extern unsigned short func_800E08B0(Object *);
extern unsigned short func_800E08F0(Object *);
extern s32 func_800E7EB4(Object *);
extern s32 func_800E7794(Object *);
extern s32 func_800E7104(Object *);
extern char *func_800A7DE4(Object *);
extern s32 func_800A4EFC(Object *, void *);
extern void *func_800A65E4(unsigned char *, Object *, Object *);
extern void func_800A665C(Object *, unsigned char *);
extern void *func_800A6CC0(Point *, Object *);
extern void func_800F06E4(Object *);

s32 func_8010656C(Object *object) {
    Point position;
    Point next_position;
    unsigned char direction;
    unsigned char *direction_ptr;
    Point *point_ptr;
    Object *target;
    s32 special;
    s32 previous;
    s32 status;
    u32 threshold;
    Point *initial = &position;
    object->unk72 &= 0xFD;
    initial->x = object->unk00.x;
    initial->y = object->unk00.y;
    previous = func_800B1C6C(initial);
    target = object->unk58;
    if (previous & 0x2000) {
        special = 1;
    } else {
        special = 0;
    }
    status = func_800A6420(object, target) & 0xFF;
    switch (status) {
    case 3:
        if (special) {
            unsigned short first = func_800E08B0(object);
            previous = (first ^ func_800E08F0(object)) & 0xFFFF;
            if (previous) {
                return 0;
            }
        }
        threshold = func_800E08B0(object) & 0xFFFF;
        if (object->unk8A >= threshold && func_800E7EB4(object) != 0) {
            return 1;
        }
        break;
    case 0:
        threshold = func_800E08B0(object) & 0xFFFF;
        if (object->unk8A < threshold) {
            func_800F06E4(object);
            return 0;
        }
        /* A ready target follows the normal movement path. */
    default:
        threshold = func_800E08B0(object) & 0xFFFF;
        if (object->unk8A >= threshold) {
            direction_ptr = &direction;
            func_800A65E4(direction_ptr, object, target);
            func_800A665C(object, direction_ptr);
            if (special) {
                point_ptr = &next_position;
                func_800A6CC0(point_ptr, object);
                if (func_800B1C6C(point_ptr) & 0x2000) {
                    return func_800A4EFC(object, func_800A7DE4(object));
                }
                if (!(func_800A6420(object, target) & 0xFF)) {
                    func_800F06E4(object);
                }
                return 0;
            }
            point_ptr = &object->unk64;
            if (!(func_800B1C6C(point_ptr) & 0x2000)) {
                object->unk64.x = 0;
                point_ptr->y = 0;
            }
            if (func_800E7EB4(object) != 0) {
                return 1;
            }
            return func_800E7794(object);
        }
        break;
    }
    return func_800E7104(object);
}

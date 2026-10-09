#include "common.h"

typedef struct { s32 x, y; } Point;
typedef struct {
    Point unk00;
    unsigned char unk08[0x16];
    unsigned char unk1E;
    unsigned char unk1F[0xE5];
    void *unk104;
} Object;
extern u32 D_8013960C;
extern s32 func_800A5388(Object *, s32);
extern s32 func_800A6FD0(Object *);
extern char *func_800A3B20(Object *);
extern s32 func_800E2074(Object *);
extern s32 func_8005DFE8(unsigned char *);
extern void func_800E370C(Object *, signed char *);
extern s32 func_80049CB4(s32, ...);
extern void func_80049AE8(s32, ...);

s32 func_800A5440(Object *object, Object *target, unsigned char *direction, s32 kind, char *item) {
    Point position;
    Point *point;
    signed char opposite;
    s32 result;
    s32 message;
    s32 blocked;
    s32 hidden;
    result = func_800A5388(object, kind & 0xFFFF);
    hidden = 0;
    if ((target != 0 && func_800A6FD0(target) != 0) || func_800A6FD0(object) != 0) {
        hidden = 1;
    }
    if (!hidden) {
        point = &position;
        point->x = object->unk00.x;
        point->y = object->unk00.y;
        message = func_80049CB4(0xDA, point);
    } else {
        message = -1;
    }
    opposite = (*direction + 4) & 7;
    if (result != 1) {
        if (result == 2) {
            if (object->unk1E & 0x7C) {
                func_80049CB4(0x12F, 4);
                func_800E370C(object, &opposite);
            }
            func_80049CB4(0x66, object);
            D_8013960C = (D_8013960C * 2) | 1;
            func_80049AE8(0x51, message, func_800A3B20(object), item);
            D_8013960C >>= 1;
        }
        return result;
    }
    D_8013960C = (D_8013960C * 2) | 1;
    func_80049CB4(6);
    if (target != 0 && func_800A6FD0(target) != 0) {
        func_80049AE8(0x5A, message, item);
    } else {
        blocked = 0;
        if (object->unk1E & 0xC) {
            if (func_800E2074(object) == 0 || (((object->unk1E >> 2) & 1) && object->unk104 != 0)) {
                blocked = 1;
            }
            if (!blocked) {
                if (func_8005DFE8((unsigned char *)item) < 0x34) {
                    func_80049AE8(0x58, message, func_800A3B20(object), item);
                } else {
                    func_80049AE8(0x59, message, func_800A3B20(object), item);
                }
            } else {
                func_80049AE8(0x5B, message, item, func_800A3B20(object));
            }
        } else {
            func_80049AE8(0x5B, message, item, func_800A3B20(object));
        }
    }
    func_80049CB4(7);
    D_8013960C >>= 1;
    func_80049CB4(0x15, object);
    if (object->unk1E & 0x7C) {
        func_800E370C(object, &opposite);
    }
    return result;
}

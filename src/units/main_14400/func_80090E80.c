#include "common.h"

typedef unsigned char u8;
typedef float f32;

typedef struct { f32 x; f32 y; } Point;

/* 'FUNC' animation curve (see func_80091280): target channel at +0x19,
 * interpolation mode at +0x1A (1 = linear, otherwise step), sorted keys at +0x20. */
typedef struct {
    u8 pad_00[0x19];
    u8 channel;
    u8 interpolation;
    u8 pad_1B;
    s32 count;
    Point *points;
} Curve;

/* Animated object: the curve time is read from +0x1A8. */
typedef struct {
    u8 pad_000[0x1A8];
    f32 time;
} Target;

typedef struct Source Source;

extern void func_80090BD8(Target *target, f32 value);
extern void func_80090BE0(Target *target, f32 value);
extern void func_80090BE8(Target *target, f32 value);
extern void func_80090BF0(Target *target, f32 value);
extern void func_80090BF8(Target *target, f32 value);
extern void func_80090C00(Target *target, f32 value);
extern void func_80090C08(Target *target, f32 value);
extern void func_80090C10(Target *target, f32 value);
extern void func_80090C18(Target *target, f32 value);
extern void func_80090C20(Target *target, u8 index, f32 value);
extern void func_80090CDC(Target *target, u8 channel, f32 value);
extern void func_80090D98(Target *target, f32 value);
extern void func_80090DB0(Target *target, f32 value);
extern void func_80090DC8(Target *target, Source *source, f32 value);
extern void func_80090E44(Target *target, f32 value);

/* FUNC resource apply callback (+0xC): evaluates the curve at the target's time and
 * stores the value into the selected channel. */
s32 func_80090E80(Curve *curve, Source *source, Target *target)
{
    f32 t = target->time;
    Point *point = curve->points;
    s32 count = curve->count;
    f32 value;

    if (t <= point->x) {
        value = point->y;
    } else if (point[count - 1].x <= t) {
        value = point[count - 1].y;
    } else {
        s32 i;

        for (i = 0; i < count; i++, point++) {
            if (!(point->x <= t)) break;
        }
        i--;
        if (curve->interpolation == 1) {
            if (t == curve->points[i].x || i == count - 1) {
                value = curve->points[i].y;
            } else {
                f32 y = curve->points[i].y;

                value = y + (curve->points[i + 1].y - y) * (t - curve->points[i].x) / (curve->points[i + 1].x - curve->points[i].x);
            }
        } else {
            value = curve->points[i].y;
        }
    }
    switch (curve->channel) {
    case 0: func_80090BD8(target, value); break;
    case 1: func_80090BE0(target, value); break;
    case 2: func_80090BE8(target, value); break;
    case 3: func_80090BF0(target, value); break;
    case 4: func_80090BF8(target, value); break;
    case 5: func_80090C00(target, value); break;
    case 6: func_80090C08(target, value); break;
    case 7: func_80090C10(target, value); break;
    case 8: func_80090C18(target, value); break;
    case 16: func_80090C20(target, 0, value); break;
    case 17: func_80090C20(target, 1, value); break;
    case 18: func_80090C20(target, 2, value); break;
    case 19: func_80090C20(target, 3, value); break;
    case 20: func_80090CDC(target, 0, value); break;
    case 21: func_80090CDC(target, 1, value); break;
    case 22: func_80090CDC(target, 2, value); break;
    case 23: func_80090CDC(target, 3, value); break;
    case 32: func_80090D98(target, value); break;
    case 33: func_80090DB0(target, value); break;
    case 48: func_80090DC8(target, source, value); break;
    case 64: func_80090E44(target, value); break;
    }
    return 0;
}

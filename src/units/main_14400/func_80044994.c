#include "common.h"

typedef struct { s32 x, y; } Vec2;
typedef struct Record Record;
typedef struct { s32 index; s32 field4; } Iterator;
/* Player actor; its map position is the leading Vec2. */
extern Vec2 *D_801476B8;
extern s32 func_800A8F6C(Iterator *iterator);
extern Vec2 *func_800A910C(Iterator *iterator);
extern s32 func_800A7D20(Vec2 *target);
extern s32 func_800A23E8(Vec2 *from, Vec2 *to);

/* Finds the eligible actor position closest to the player (distance 1 ends the
 * search early) and stores it in *position. The record argument passed by
 * func_80044940 is not read. */
s32 func_80044994(Record *record, Vec2 *position)
{
    Vec2 candidate;
    Vec2 origin;
    Vec2 probe;
    Iterator iterator;
    Vec2 *view;
    s32 best = 100;
    Iterator *it = &iterator;
    Vec2 *dst = &candidate;

    dst->x = position->x;
    dst->y = position->y;
    origin.x = D_801476B8->x;
    origin.y = D_801476B8->y;
    view = &candidate;
    it->index = 0;
    while (func_800A8F6C(it)) {
        Vec2 *target = func_800A910C(it);
        s32 distance;

        if (!func_800A7D20(target)) {
            continue;
        }
        candidate = *target;
        probe.x = candidate.x;
        probe.y = view->y;
        distance = func_800A23E8(&origin, &probe);
        if (distance < 2) {
            best = 1;
            *position = candidate;
            break;
        }
        if (distance < best) {
            *position = candidate;
            best = distance;
        }
    }
    return best != 100;
}

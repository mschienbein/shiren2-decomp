#include "common.h"

typedef struct { s32 x, y; } Pair;
typedef struct { Pair origin, end; } Rect;
typedef struct {
    s32 field_00;
    s32 field_04;
    Pair field_08;
    Rect field_10;
} UnitIter;
typedef struct Unit Unit;
extern UnitIter *func_800A9244(UnitIter *obj, Rect *quad, Pair *pair);
extern s32 func_800A9284(UnitIter *iter, s32 mask);
extern Unit *func_800A942C(UnitIter *iter);
extern void func_800A5A04(Unit *object);

void func_800B4A70(Rect *rect) {
    UnitIter iter;
    UnitIter *p;
    func_800A9244(&iter, rect, &rect->origin);
    for (p = &iter; func_800A9284(p, 0xFF); ) {
        func_800A5A04(func_800A942C(p));
    }
}

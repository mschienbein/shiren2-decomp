#include "map_unit_iterator.h"

typedef struct Unit Unit;
extern void func_800A5A04(Unit *object);

void func_800B4A70(MapRect *rect) {
    MapUnitIterator iter;
    MapUnitIterator *p;
    func_800A9244(&iter, rect, &rect->min);
    for (p = &iter; func_800A9284(p, 0xFF); ) {
        func_800A5A04(func_800A942C(p));
    }
}

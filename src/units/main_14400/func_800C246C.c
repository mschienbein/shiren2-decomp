#include "map_position_iterator.h"
/* Whole 16-byte floor rectangle at D_801429C0 (D_801429C4/C8/CC are its interior words). */
extern MapRect D_801429C0;
extern unsigned char D_80143392, D_8014344C;
extern unsigned char D_801431F0[];
extern void func_800B1080(void);
extern void func_800B1AE0(MapPos *position, unsigned short value);
extern void func_800B6590(void *room, MapPos *first, MapPos *last);
extern void func_800B17A4(void);
extern s32 func_80049CB4(s32 id, ...);
static inline void set_position(MapPos *position, s32 row, s32 col) {
    position->x = row;
    position->y = col;
}
/* Slot +0x0C of mode vtable D_8014A7A8 (0x8014A7B4). The dispatcher func_800AA9FC supplies the
 * adjusted receiver at 0x800AAB94 and discards v0; this method never reads self. */
void func_800C246C(void *self) {
    MapPositionIterator iterator;
    MapPos current, last;
    MapPositionIterator *scan;
    MapPos *position;
    func_800B1080();
    scan = &iterator;
    position = &current;
    D_80143392 = 0;
    set_position(&current, D_801429C0.min.x, D_801429C0.min.y);
    iterator.start = current;
    iterator.cur = iterator.start;
    set_position(&current, D_801429C0.max.x, D_801429C0.max.y);
    iterator.end = current;
    for (;;) {
        s32 row = iterator.cur.x;
        s32 active = scan->end.x < row;
        active ^= 1;
        if (!active) break;
        func_800A3610(position, scan);
        func_800B1AE0(position, 0x1200);
    }
    D_8014344C = 1;
    {
        MapPos *start = &current;
        MapPos *end = &last;
        start->x = 10; start->y = 10;
        end->x = 43; end->y = 65;
        func_800B6590(D_801431F0, start, end);
    }
    func_800B17A4();
    func_80049CB4(5, 1);
}

#include "common.h"
typedef struct { s32 row, col; } Position;
typedef struct { Position current, first, last; } Iterator;
/* Whole 16-byte floor rectangle at D_801429C0 (D_801429C4/C8/CC are its interior words). */
typedef struct { Position first, last; } Rect;
extern Rect D_801429C0;
extern unsigned char D_80143392, D_8014344C;
extern unsigned char D_801431F0[];
extern void func_800B1080(void);
extern Position *func_800A3610(Position *out, Iterator *iterator);
extern void func_800B1AE0(Position *position, unsigned short value);
extern void func_800B6590(void *room, Position *first, Position *last);
extern void func_800B17A4(void);
extern s32 func_80049CB4(s32 id, ...);
static inline void set_position(Position *position, s32 row, s32 col) {
    position->row = row;
    position->col = col;
}
/* Slot +0x0C of mode vtable D_8014A7A8 (0x8014A7B4). The dispatcher func_800AA9FC supplies the
 * adjusted receiver at 0x800AAB94 and discards v0; this method never reads self. */
void func_800C246C(void *self) {
    Iterator iterator;
    Position current, last;
    Iterator *scan;
    Position *position;
    func_800B1080();
    scan = &iterator;
    position = &current;
    D_80143392 = 0;
    set_position(&current, D_801429C0.first.row, D_801429C0.first.col);
    iterator.first = current;
    iterator.current = iterator.first;
    set_position(&current, D_801429C0.last.row, D_801429C0.last.col);
    iterator.last = current;
    for (;;) {
        s32 row = iterator.current.row;
        s32 active = scan->last.row < row;
        active ^= 1;
        if (!active) break;
        func_800A3610(position, scan);
        func_800B1AE0(position, 0x1200);
    }
    D_8014344C = 1;
    {
        Position *start = &current;
        Position *end = &last;
        start->row = 10; start->col = 10;
        end->row = 43; end->col = 65;
        func_800B6590(D_801431F0, start, end);
    }
    func_800B17A4();
    func_80049CB4(5, 1);
}

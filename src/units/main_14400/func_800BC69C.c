#include "common.h"
typedef struct { s32 unk0, unk4; } Pair;
typedef struct { Pair first, last; } Rect;
typedef struct { s32 unk0[3]; s32 unkC, unk10; } Iterator;
extern Rect D_801429C0;
extern s32 func_800A24DC(Pair *, Rect *);
extern u32 func_800B1C6C(Pair *);
extern void func_800B1AE0(Pair *, unsigned short);
extern void func_800C25F4(Iterator *, Rect *, s32, s32);
extern Pair *func_800C2758(Pair *, Iterator *);
static inline void init_iterator(Iterator *iterator) { iterator->unk10 = 0; iterator->unkC = 0; }
static inline s32 iterator_valid(Iterator *iterator) { return iterator->unk10 < iterator->unkC; }
static inline unsigned short is_active(Pair *position) { return (func_800B1C6C(position) & 0xE100) != 0; }
/* The owner is supplied by the room-update contract but is unused by this edge operation. */
void func_800BC69C(void *owner, Rect *rect, s32 arg2) {
    Iterator iterator;
    Pair position;
    s32 flags = 0x4000;
    s32 direction;
    if (arg2) flags = 0x4020;
    direction = 0;
    init_iterator(&iterator);
    for (;;) {
        s32 in_range = direction < 4;
        if (!in_range) break;
        func_800C25F4(&iterator, rect, direction, 1);
        for (;;) {
            s32 has_next = iterator_valid(&iterator);
            s32 active;
            if (!has_next) break;
            func_800C2758(&position, &iterator);
            active = 0;
            if (func_800A24DC(&position, &D_801429C0)) active = is_active(&position);
            if (active) func_800B1AE0(&position, flags & 0xFFFF);
        }
        direction++;
    }
}

#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;

typedef struct { s32 x; s32 y; } Pos800AC6F8;
typedef struct { Pos800AC6F8 min; Pos800AC6F8 max; } Rect800AC6F8;
typedef struct { Pos800AC6F8 cur; Pos800AC6F8 start; Pos800AC6F8 end; } Iter800AC6F8;
typedef struct { s32 w[2]; } ListIter800AC6F8;
typedef struct { u8 value; } Dir;
typedef struct { Pos800AC6F8 pos; Dir dir; } Move800AC6F8;
typedef struct { u8 kind; u8 subkind; s8 flags; u8 pad3[2]; s8 owner; } Item800AC6F8;
extern u8 D_80142F20;
/* Whole RNG object (state pointers plus backing words); only its base is passed here. */
extern u8 D_80147620[];
void *func_800B07F0(ListIter800AC6F8 *li);
s32 func_800B0808(ListIter800AC6F8 *li);
Item800AC6F8 *func_800B0864(ListIter800AC6F8 *li);
void *func_800A3610(Pos800AC6F8 *out, Iter800AC6F8 *it);
Item800AC6F8 *func_800B4D80(Pos800AC6F8 *pos);
u32 func_800B1C6C(Pos800AC6F8 *pos);
u8 func_800C57CC(void *rng, u8 range);
void *func_800A2594(Pos800AC6F8 *out, Pos800AC6F8 *from, Dir dir);
s32 func_800AD714(Item800AC6F8 *item, Pos800AC6F8 *dest);
s32 func_800AD8AC(Item800AC6F8 *item, Pos800AC6F8 *dest);
void *func_800B4E18(Pos800AC6F8 *pos);
s32 func_80049CB4(s32 msg, ...);
void func_800AC6F8(Rect800AC6F8 *rect, s32 allowSpecial) {
    ListIter800AC6F8 li;
    Iter800AC6F8 it;
    Pos800AC6F8 pos;
    Move800AC6F8 move;
    Item800AC6F8 *item;
    s32 skip;
    s32 inDungeon;

    func_800B07F0(&li);
    while (func_800B0808(&li)) {
        func_800B0864(&li)->flags &= ~1;
    }
    pos.x = rect->min.x;
    pos.y = rect->min.y;
    it.start = pos;
    it.cur = it.start;
    inDungeon = (D_80142F20 & 0xE0) == 0x20;
    pos.x = rect->max.x;
    pos.y = rect->max.y;
    it.end = pos;
    while (1) {
        s32 more = it.cur.x <= it.end.x;
        if (!more) {
            break;
        }
        func_800A3610(&pos, &it);
        item = func_800B4D80(&pos);
        skip = item == 0
            || ~item->owner != 0
            || (item->flags & 0x20)
            || (item->flags & 1)
            || item->kind == 0x10
            || (item->kind == 0x13 && item->subkind != 0xF2)
            || (item->kind == 0xF && (allowSpecial == 0 || inDungeon == 0))
            || (func_800B1C6C(&pos) & 0x4000);
        if (skip) {
            continue;
        }
        move.dir.value = func_800C57CC(D_80147620, 7) & 7;
        func_800A2594(&move.pos, &pos, move.dir);
        if (func_800AD714(item, &move.pos) == 0) {
            continue;
        }
        func_80049CB4(6);
        func_800B4E18(&pos);
        func_80049CB4(0xE1, &pos);
        if (func_800B1C6C(&pos) & 0x2000) {
            if (func_800B1C6C(&move.pos) & 0x2000) {
                func_80049CB4(0xC1, item, &pos, &move.pos);
            } else {
                func_80049CB4(6);
                func_80049CB4(0x10B, &pos);
                func_80049CB4(7);
                func_80049CB4(0xC0, item, &pos, &move.pos);
            }
        } else {
            func_80049CB4(0xC0, item, &pos, &move.pos);
        }
        item->flags |= 0x40;
        func_800AD8AC(item, &move.pos);
        func_80049CB4(0xD7, &pos);
        func_80049CB4(7);
        item->flags |= 1;
    }
}

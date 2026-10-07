#include "common.h"
typedef unsigned char u8;
typedef unsigned short u16;
typedef struct { s32 x; s32 y; } Pos;
typedef struct { Pos cur; Pos start; Pos end; } PosIter;
typedef struct { short delta; short index; void (*fn)(void *, s32); } VtEntry;
typedef struct { u8 kind; u8 unk1; u8 flags2; char pad3[5]; VtEntry *vt; } Item;
typedef struct { char pad[0x1C]; u16 flags1C; u8 state1E; } Unit;
typedef struct Owner Owner;
/* 0x10-byte state: func_800D459C stores room at +0 and words at +4/+8/+C. */
typedef struct { void *room; s32 opaque[3]; } RoomState;
extern u8 D_80142F20;
extern u32 D_8013960C;
typedef struct { Pos start; Pos end; } Rect;
extern Rect D_801429C0;
extern Unit *D_801476B8;
extern s32 D_801486E0;
extern u16 D_8014767C;
extern u8 D_80143391;
extern RoomState D_80143434;
extern u8 D_80156A75;
void func_800498E4(s32 message_id, ...);
void func_80126390(Owner *);
void func_80126458(Owner *);
void func_80045A44(void);
void func_800B512C(s32);
s32 func_80049CB4(s32, ...);
void *func_800A3610(void *out, void *it);
Item *func_800B4D80(Pos *);
s32 func_8012633C(Owner *, Item *);
void func_80112E7C(Item *);
void func_8010E2CC(Item *);
void func_800AD868(Pos *);
void *func_800A33DC(void *out, void *rect);
s32 func_800AD714(Item *, Pos *);
u32 func_800B1C6C(void *pos);
void func_800AD7E0(Item *, Pos *, s32);
Unit *func_800B4928(Pos *);
s32 func_8012640C(Owner *, Unit *);
s32 func_800A6E90(Unit *);
void func_800A59A4(Unit *);
s32 func_800A6184(Unit *, Pos *);
void func_801263B8(Owner *, Unit *);
Unit *func_800C5F60(void);
void func_800E35A8(Unit *, void *, u8, s32, s32);
s32 func_800E91E4(Unit *);
s32 func_800E9144(Unit *);
void func_800B261C(void);
s32 func_800B5BDC(Unit *);
s32 func_800D49C0(void *);
s32 func_80045924(void);
static inline s32 Last(void){ return -1; }
static inline void PosIter_set_start(PosIter *it, Pos *p, s32 x, s32 y) {
    p->x = x;
    p->y = y;
    it->start = *p;
    it->cur = it->start;
}
static inline void PosIter_set_end(PosIter *it, Pos *p, s32 x, s32 y) {
    p->x = x;
    p->y = y;
    it->end = *p;
}
static inline s32 PosIter_more(PosIter *it){ return it->cur.x <= it->end.x; }
/* Trap slot +0x44 receives self, actor, source/effect positions, direction, target
 * unit and item. This override ignores arg2 and arg4..arg6, but retains the contract. */
s32 func_80126480(Owner *self, void *arg1, Pos *arg2, Pos *arg3, void *arg4, void *arg5, void *arg6){
    PosIter it;
    Pos pos;
    Pos dest;
    Item *item;
    Unit *unit;
    s32 i, ok, usable;
    s32 wrongMode = (D_80142F20 & 0xE0) ^ 0x20;
    if (wrongMode) {
        func_800498E4(0x223);
        return 1;
    }
    func_80126390(self);
    func_80126458(self);
    func_80045A44();
    func_800B512C(1);
    func_80049CB4(0x10EC, arg3);
    func_80049CB4(0xDD);
    D_8013960C <<= 1;
    PosIter_set_start(&it, &pos, D_801429C0.start.x, D_801429C0.start.y);
    PosIter_set_end(&it, &pos, D_801429C0.end.x, D_801429C0.end.y);
    while (PosIter_more(&it)) {
        func_800A3610(&pos, &it);
        item = func_800B4D80(&pos);
        if (item) {
            s32 kept = func_8012633C(self, item) ^ 1;
            if (kept) {
                u8 kind = item->kind;
                if (kind == 2) func_80112E7C(item);
                else if (kind == 8) func_8010E2CC(item);
                if (!(item->flags2 & 0x20) && kind != 0x10 && kind != 0xF && kind != 0x13) {
                    func_800AD868(&pos);
                    for (i = 100; --i != Last();) {
                        Pos r;
                        func_800A33DC(&r, &D_801429C0);
                        dest = r;
                        ok = 0;
                        if (func_800AD714(item, &dest)) {
                            if (!(func_800B1C6C(&dest) & 0x2000)) ok = 1;
                        }
                        if (ok) {
                            func_800AD7E0(item, &dest, 1);
                            break;
                        }
                    }
                    if (i < 0 && item) item->vt[1].fn((char *)item + item->vt[1].delta, 3);
                }
            }
        }
        unit = func_800B4928(&pos);
        usable = 0;
        if (unit && !func_8012640C(self, unit) && !(unit->flags1C & 1)) usable = func_800A6E90(unit) == 0;
        if (!usable) continue;
        func_800A59A4(unit);
        if (!func_800A6184(unit, &pos)) continue;
        func_801263B8(self, unit);
        if (unit == func_800C5F60()) continue;
        if (!(unit->state1E & 0x7C)) continue;
        func_800E35A8(unit, arg1, D_80156A75, 0xB, 0);
    }
    D_8013960C >>= 1;
    func_800E91E4(D_801476B8);
    func_800E9144(D_801476B8);
    D_801486E0 = 1;
    func_80049CB4(0xA9, func_800C5F60());
    func_80049CB4(0x93, func_800C5F60());
    unit = func_800C5F60();
    {
        s32 idle = (unit->flags1C & 1) ^ 1;
        if (idle) func_800E35A8(unit, arg1, D_80156A75, 0xB, 0);
    }
    func_800B261C();
    if (D_8014767C & 0xC) {
        func_80049CB4(0x126, 0x20);
    } else {
        s32 special = 0;
        if (D_80143391 & 4) special = func_800B5BDC(unit) != 0;
        if (special) func_80049CB4(0x126, 0x1F);
        else {
            s32 missing = func_800D49C0(&D_80143434) ^ 1;
            if (missing) func_80049CB4(0x126, func_80045924());
        }
    }
    func_80049CB4(2);
    D_8014767C |= 0x40;
    return 1;
}

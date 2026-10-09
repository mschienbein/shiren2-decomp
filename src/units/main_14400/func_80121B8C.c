#include "common.h"

typedef unsigned char u8;

typedef struct Pos80121B8C {
    s32 x;
    s32 y;
} Pos80121B8C;

/* Complete 16-byte rectangle at D_801429C0. */
typedef struct {
    Pos80121B8C position;
    Pos80121B8C size;
} Rect80121B8C;

/* Rectangle walker consumed by func_800A3610 (cursor, row start, end corner). */
typedef struct Range80121B8C {
    Pos80121B8C cur;
    Pos80121B8C start;
    Pos80121B8C end;
} Range80121B8C;

/* Container item iterator (func_800CEB20 / func_800CEBA0 / func_800CEC68). */
typedef struct ListIter80121B8C {
    s32 index;
    void *owner;
    s32 step;
    void *current;
} ListIter80121B8C;

typedef struct Item800F0440 {
    u8 pad_00[0x2C];
    u8 slot_2C;
} Item800F0440;

typedef struct Obj Obj;

extern s32 D_8013960C;
extern Rect80121B8C D_801429C0;
s32 func_800A8F6C(s32 *p);
void *func_800A910C(s32 *it);
void *func_800A6DE4(void *obj);
ListIter80121B8C *func_800CEB20(ListIter80121B8C *s, void *a);
extern s32 func_800CEBA0(ListIter80121B8C *);
extern Item800F0440 *func_800CEC68(ListIter80121B8C *);
char *func_800AE674(void *obj);
void func_800498E4(s32 id, ...);
extern void func_80049BF0(s32 mode);
void func_800D3650(void *arg);
void func_800CD3D0(void *list, u32 index);
Pos80121B8C *func_800A3610(Pos80121B8C *out, Range80121B8C *it);
void *func_800B4D80(Pos80121B8C *p);
s32 func_80049CB4(s32 id, ...);
extern void func_800B4E7C(Pos80121B8C *);


static inline s32 coordinate(const s32 *value)
{
    return *value;
}
/* Removes `item` from whichever container or floor cell holds it; 0 when not found. */
s32 func_80121B8C(Item800F0440 *item) {
    s32 cursor = 0;

    while (func_800A8F6C(&cursor)) {
        void *owner = func_800A6DE4(func_800A910C(&cursor));

        if (owner != 0) {
            ListIter80121B8C iter;

            func_800CEB20(&iter, owner);
            while (func_800CEBA0(&iter)) {
                if (func_800CEC68(&iter) == item) {
                    if (D_8013960C & 1) {
                        func_800498E4(0xA9, func_800AE674(item));
                        func_80049BF0(1);
                    }
                    func_800D3650(item);
                    func_800CD3D0(owner, iter.index + 1);
                    return 1;
                }
            }
        }
    }
    {
        Pos80121B8C pos;
        Range80121B8C range;

        pos.x = coordinate(&D_801429C0.position.x);
        pos.y = coordinate(&D_801429C0.position.y);
        range.start = pos;
        range.cur = range.start;
        pos.x = coordinate(&D_801429C0.size.x);
        pos.y = coordinate(&D_801429C0.size.y);
        range.end = pos;
        for (;;) {
            s32 valid = range.cur.x <= range.end.x;

            if (!valid) {
                break;
            }
            func_800A3610(&pos, &range);
            if (func_800B4D80(&pos) == item) {
                if (D_8013960C & 1) {
                    func_80049CB4(6);
                    func_80049CB4(0x107, &pos);
                    func_80049CB4(0xE1, &pos);
                    func_80049CB4(7);
                    func_800498E4(0xA9, func_800AE674(item));
                    func_80049BF0(1);
                }
                func_800D3650(item);
                func_800B4E7C(&pos);
                func_80049CB4(0xD7, &pos);
                return 1;
            }
        }
    }
    item->slot_2C = 0xFF;
    return 0;
}

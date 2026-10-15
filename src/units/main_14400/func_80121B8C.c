#include "common.h"
#include "collection_iterator.h"
#include "item_records.h"
#include "map_position_iterator.h"

typedef unsigned char u8;

/* Container item iterator (func_800CEB20 / func_800CEBA0 / func_800CEC68). */


typedef struct Obj Obj;

extern s32 D_8013960C;
extern MapRect D_801429C0;
s32 func_800A8F6C(s32 *p);
void *func_800A910C(s32 *it);
void *func_800A6DE4(void *obj);
void func_800498E4(s32 id, ...);
extern void func_80049BF0(s32 mode);
void func_800D3650(void *arg);
void func_800CD3D0(void *list, u32 index);
void *func_800B4D80(MapPos *p);
s32 func_80049CB4(s32 id, ...);
extern void func_800B4E7C(MapPos *);


static inline s32 coordinate(const s32 *value)
{
    return *value;
}
/* Removes `item` from whichever container or floor cell holds it; 0 when not found. */
s32 func_80121B8C(Item *item) {
    s32 cursor = 0;

    while (func_800A8F6C(&cursor)) {
        void *owner = func_800A6DE4(func_800A910C(&cursor));

        if (owner != 0) {
            CollectionIterator iter;

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
        MapPos pos;
        MapPositionIterator range;

        pos.x = coordinate(&D_801429C0.min.x);
        pos.y = coordinate(&D_801429C0.min.y);
        range.start = pos;
        range.cur = range.start;
        pos.x = coordinate(&D_801429C0.max.x);
        pos.y = coordinate(&D_801429C0.max.y);
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
    item->payload.pot.field2C = 0xFF;
    return 0;
}

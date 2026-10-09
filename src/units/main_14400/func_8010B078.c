#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

typedef struct { s32 x, y; } Pos;

typedef struct {
    Pos pos;
} Unit8010B078;

typedef struct {
    u8 pad0[6];
    u8 field_6;
} Data8010B078;

/* Line iterator filled by func_800C5280 and advanced by func_800C559C/func_800C532C. */
typedef struct {
    s32 words[6];
} Iterator8010B078;

/* The 0x18-byte stack record contains owner/vtable/link pointers, not integer words.
 * Its base is initialized at 0x800C4840; the derived link is stored at 0x800C4DD0. */
typedef struct {
    void *owner_00;
    short value_04;
    u8 pad_06[2];
    s32 count_08;
    void *vtable_0C;
    void *link_10;
    u8 pad_14[4];
} Action8010B078;

typedef struct Item8010B078 Item8010B078;

extern s32 func_800EE504(Unit8010B078 *unit);
extern s32 func_800E1CC4(Unit8010B078 *obj, s32 kind);
extern s32 func_800A692C(Unit8010B078 *unit, s32 state);
extern Pos *func_8010AB7C(Unit8010B078 *ctx, Item8010B078 *item);
extern s32 func_800E0F40(Unit8010B078 *obj);
extern Data8010B078 *func_80044E24(void *unusedObject, u8 frame);
extern void *func_800C5280(Iterator8010B078 *it, Pos *src, u16 c);
extern s32 func_800C559C(Iterator8010B078 *it);
extern void *func_800C532C(Pos *value, Iterator8010B078 *it);
extern s32 func_800B1AB8(Pos *p);
extern void *func_800B4928(Pos *pos);
extern s32 func_800A6EE0(Unit8010B078 *unit);
extern void *func_800C4DA0(Action8010B078 *action, Unit8010B078 *unit, Action8010B078 *source, u16 flags);
extern void func_800C4864(Action8010B078 *action, s32 kind, void *arg);

s32 func_8010B078(Unit8010B078 *unit, Item8010B078 *item, Action8010B078 *source) {
    Pos target;
    s32 blocked = 0;

    if (!func_800EE504(unit) || func_800E1CC4(unit, 0) || func_800A692C(unit, 0x12)) {
        blocked = 1;
    }
    if (blocked) {
        return 0;
    }
    if (item == 0) {
        return 0;
    }
    {
        Pos *found_pos = func_8010AB7C(unit, item);

        if (found_pos != 0) {
            target = *found_pos;
        } else {
            Iterator8010B078 it;
            Pos cur;
            s32 found = 0;

            cur.x = unit->pos.x;
            cur.y = unit->pos.y;
            func_800C5280(&it, &cur, func_80044E24(unit, func_800E0F40(unit))->field_6);
            while (func_800C559C(&it)) {
                func_800C532C(&cur, &it);
                target = cur;
                if (func_800B1AB8(&target) && func_800B4928(&target) == 0) {
                    found = 1;
                    break;
                }
            }
            if (!found) {
                return 0;
            }
        }
    }
    {
        Action8010B078 action;

        func_800C4DA0(&action, unit, source, func_800A6EE0(unit) | 0x10);
        func_800C4864(&action, 0x29, &target);
    }
    return 1;
}

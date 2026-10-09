#include "common.h"

typedef unsigned char u8;

typedef signed char s8;

typedef struct {
    s32 x;
    s32 y;
} Point800D248C;

/* Seventeen tile offsets (nine directions, then eight repeated so that start + i
 * never wraps); two separate read-only tables, copied whole onto the stack. */
typedef struct {
    s32 v[17];
} Offsets800D248C;

/* One three-byte spawn rule per floor level (1-based). */
typedef struct {
    u8 itemKind;
    u8 extraCount;
    u8 extraChance;
} Spawn800D248C;

/* Whole 0x26-byte floor record at D_80142EF0 (same layout as the canonical FloorRecord). */
typedef struct {
    unsigned char field_00, field_01, field_02, field_03, field_04, field_05, field_06, field_07;
    unsigned char field_08, field_09, field_0A, field_0B, field_0C, field_0D;
    unsigned short field_0E;
    unsigned char field_10, field_11, field_12, field_13, field_14, field_15, field_16, field_17;
    unsigned char field_18, field_19, field_1A, field_1B, field_1C, field_1D, field_1E, field_1F;
    unsigned char field_20, field_21, field_22, field_23, field_24, field_25;
} FloorRecord;

/* Leading part of a 0x14-byte room record (func_800B1F90 returns &D_801431F0[index]). */
typedef struct {
    s32 x;
    s32 y;
} Room800D248C;

typedef struct {
    u8 pad0[4];
    Room800D248C *room4;
    u8 level8;
} Obj800D248C;

extern const Offsets800D248C D_8015474C;
extern const Offsets800D248C D_80154790;
extern FloorRecord D_80142EF0;
/* Ten three-byte spawn records (0x80156D10..0x80156D2D), indexed by floor level - 1. */
extern const Spawn800D248C D_80156D10[10];
extern u8 D_80147620[];
extern Room800D248C *func_800B1F90(Point800D248C *pos);
extern s32 func_800A3138(Room800D248C *room);
extern s32 func_800A315C(Room800D248C *room);
extern unsigned char func_800C57A0(void *rng);
extern s32 func_800C587C(void *rng, u8 chance);
extern s32 func_800C5844(void *rng, unsigned char base, unsigned char top);
extern s32 func_800A31C8(Room800D248C *room, Point800D248C *pos);
extern u32 func_800B1C6C(void *pos);
extern s32 func_800B4F74(Point800D248C *pos);
extern s32 func_800D2BD4(Obj800D248C *obj);
extern s32 func_800D36E0(Point800D248C *pos, s32 kind, s32 level);
extern void *func_800AADF8(signed char id, unsigned char value);
extern void func_800AE444(void *item, u8 kind);
extern void func_800AD7E0(void *item, Point800D248C *pos, s32 notify);

s32 func_800D248C(Obj800D248C *obj) {
    s32 result = 0;
    Room800D248C *room = obj->room4;
    Point800D248C center;
    Offsets800D248C offsY;
    Offsets800D248C offsX;
    Point800D248C pos;
    Spawn800D248C spawn;
    const Spawn800D248C *rules;
    s32 width;
    s32 height;
    s32 dx;
    s32 dy;
    s32 n;
    u8 extra;
    FloorRecord *floor;
    s32 found;
    u8 i;

    if (room == 0) {
        Point800D248C *probe = &center;

        probe->x = 10;
        probe->y = 10;
        room = func_800B1F90(probe);
    }
    width = func_800A3138(room);
    height = func_800A315C(room);
    dx = dy = 0;
    if (!(width & 1) && (func_800C57A0(D_80147620) & 1)) {
        dy--;
    }
    if (!(height & 1) && (func_800C57A0(D_80147620) & 1)) {
        dx--;
    }
    center.y = room->y + width / 2 + dy;
    center.x = room->x + height / 2 + dx;
    offsY = D_8015474C;
    offsX = D_80154790;
    floor = &D_80142EF0;
    rules = D_80156D10;
    spawn = rules[floor->field_07 - 1];
    extra = 0;
    n = spawn.extraCount;
    while (--n != -1) {
        if (func_800C587C(D_80147620, spawn.extraChance)) {
            extra++;
        }
    }
    n = floor->field_12 - extra;
    for (;;) {
        u8 start;

        if (--n == -1) {
            break;
        }
        found = 0;
        start = func_800C5844(D_80147620, 0, 8);
        for (i = 0;; i++) {
            s32 ok;

            if (i >= 9) {
                break;
            }
            ok = 0;
            pos.y = center.y + offsY.v[start + i];
            pos.x = center.x + offsX.v[start + i];
            if (func_800A31C8(room, &pos) && !(func_800B1C6C(&pos) & 0x10)) {
                ok = func_800B4F74(&pos) == 0;
            }
            if (ok) {
                found = 1;
                break;
            }
        }
        if (!found) {
            continue;
        }
        if (func_800D36E0(&pos, (s8)func_800D2BD4(obj), obj->level8) == 0) {
            continue;
        }
        result = 1;
    }
    n = extra;
    for (;;) {
        u8 start;
        void *item;

        if (--n == -1) {
            break;
        }
        found = 0;
        start = func_800C5844(D_80147620, 0, 8);
        for (i = 0;; i++) {
            s32 ok;

            if (i >= 9) {
                break;
            }
            ok = 0;
            pos.y = center.y + offsY.v[start + i];
            pos.x = center.x + offsX.v[start + i];
            if (func_800A31C8(room, &pos) && !(func_800B1C6C(&pos) & 0x10)) {
                ok = func_800B4F74(&pos) == 0;
            }
            if (ok) {
                found = 1;
                break;
            }
        }
        if (!found) {
            continue;
        }
        item = func_800AADF8(-1, 0);
        if (item == 0) {
            continue;
        }
        func_800AE444(item, spawn.itemKind);
        func_800AD7E0(item, &pos, 1);
    }
    return result;
}

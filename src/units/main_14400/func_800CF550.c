#include "common.h"

typedef unsigned char u8;
typedef struct { s32 x; s32 y; } Point;
typedef struct { u8 field_00; u8 field_01; u8 field_02; } Item;
typedef struct {
    Point field_00;
    u8 field_08;
    u8 field_09[0x15];
    u8 field_1E;
} Entity;
typedef struct {
    u8 field_00[0x20];
    short field_20;
    short field_22;
    s32 (*field_24)(void *);
    u8 field_28[0x10];
    short field_38;
    short field_3A;
    Item *(*field_3C)(void *, u32);
} Methods;
typedef struct {
    s32 field_00;
    Methods *field_04;
    u8 field_08[8];
    Entity *field_10;
} Object;
typedef struct { u8 field_00[0x18]; } Iterator;
typedef signed char s8;
typedef struct ShirenDirection { s8 value; } ShirenDirection;
extern u8 D_80147620[], D_8015692B, D_8015692D;
extern s32 func_800A692C(Entity *, s32);
extern void *func_800A6CC0(void *out_position, void *obj);
extern void func_800C27D0(void *iterator, void *origin, ShirenDirection direction, u8 limit, u8 mode);
extern u32 func_800CF8E0(Object *, Iterator *);
extern u32 func_800CF9E4(Object *, Iterator *);
extern s32 func_800C5844(void *rng, u8 base, u8 top);
extern void func_800C2810(Iterator *);
extern s32 func_800C28EC(Iterator *);
extern void *func_800C28FC(void *out, void *iterator);
extern s32 func_800B4F74(Point *);
extern s32 func_800C5A1C(void *, s32, s32);
extern s32 func_800AD714(Item *, Point *);
extern void func_800CD304(void *list, u32 index);
extern s32 func_80049CB4(s32 id, ...);
extern s32 func_800ADB94(Item *, Entity *, Point *, s32);

static inline s32 can_place(Item *item, Object *object, Point *point) {
    s32 allowed = 0;
    if (!(item->field_02 & 4)) {
        if (!(object->field_10->field_1E & 0xC) || item->field_01 != 0xB0) {
            if (func_800AD714(item, point)) allowed = 1;
        }
    }
    return allowed;
}

static inline void position_of(Point *point, Entity *entity) {
    point->x = entity->field_00.x;
    point->y = entity->field_00.y;
}

static inline void load_direction(ShirenDirection *direction, Entity *entity) {
    direction->value = entity->field_08;
}

static inline void initialize_iterator(Iterator *iterator, Point *center, ShirenDirection direction) {
    func_800C27D0(iterator, center, direction, 4, 1);
}

void func_800CF550(Object *object) {
    Point origin;
    Point center;
    Iterator iterator;
    Point point;
    ShirenDirection direction;
    u32 skipped;
    s32 limit;
    s32 special;
    s32 position;
    s32 placed;
    Item *item;
    if (func_800A692C(object->field_10, 0x12)) return;
    position_of(&origin, object->field_10);
    func_800A6CC0(&center, object->field_10);
    load_direction(&direction, object->field_10);
    initialize_iterator(&iterator, &center, direction);
    if ((direction.value ^ 1) & 1) skipped = func_800CF8E0(object, &iterator);
    else skipped = func_800CF9E4(object, &iterator);
    position = -1;
    placed = 0;
    special = 0;
    limit = (u8)func_800C5844(D_80147620, D_8015692B, D_8015692D);
    item = 0;
    func_800C2810(&iterator);
    for (;;) {
        s32 active = func_800C28EC(&iterator);
        s32 count;
        s32 index;
        s32 remaining;
        u8 type;
        s32 announce;
        s32 failed;
        if (!active) break;
        func_800C28FC(&point, &iterator);
        position++;
        if (skipped & (1U << position)) continue;
        if (func_800B4F74(&point)) continue;
        count = object->field_04->field_24((u8 *)object + object->field_04->field_20);
        if (!count) break;
        index = func_800C5A1C(D_80147620, 0, count - 1);
        remaining = count;
        for (;;) {
            s32 allowed;
            remaining--;
            if (remaining == -1) break;
            item = object->field_04->field_3C((u8 *)object + object->field_04->field_38, index);
            allowed = can_place(item, object, &point);
            if (allowed) break;
            index++;
            if (index >= count) index = 0;
        }
        if (remaining < 0) break;
        func_800CD304(object, index);
        placed++;
        type = item->field_00;
        announce = type != 9 || !special;
        func_80049CB4(6);
        func_80049CB4(0xB9, item, &origin, &point);
        failed = func_800ADB94(item, object->field_10, &point, announce) != 1;
        if (failed && type == 9) special = 1;
        func_80049CB4(7);
        if (placed >= limit) break;
    }
}
